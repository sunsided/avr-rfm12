/*
 * main.c
 *
 * Created: 02.06.2013 21:09:43
 *  Author: sunside
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "system.h"

#include "comm/usart_comm.h"
#include "comm/command_decoder.h"

#include "main.h"
#include "spi/SpiMaster.hpp"
#include "rfm12/Rfm12.hpp"

#include "Rfm12SpiAdapter.h"

#include "ringbuffer/RingBuffer.h"

using namespace rfm12;

#include <util/delay.h> 

#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_PIN			PINB

#define SPI_BIT_MOSI	PORTB3
#define SPI_BIT_MISO	PINB4
#define SPI_BIT_SCK		PORTB5
#define SPI_BIT_SS		PORTB2

/**
* \brief Endianess-Überprüfung der Bitfields
*/
void checkEndianessAndHangUpOnError()
{
	commands::PowerManagementCommand powerMgmt;
	powerMgmt.eb = true;
	powerMgmt.ex = false;
	powerMgmt.dc = true;
	
	// if endianness is reversed, hang up.
	if (0b0101 != (powerMgmt.command_word & 0b0101)) 
	{
		while(1) { led_doubleflash_sync(); }
	}
}

int main()
{
	rbdata_t trololo[16];
	RingBuffer buffer(trololo, 3);
	
	rbdata_t value;
	bool success = buffer.tryRead(value);
	assert (false == success);
	
	success = buffer.tryWrite(227);
	assert(true == success);
	
	success = buffer.tryWrite(42);
	assert(true == success);
	
	success = buffer.tryWrite(43);
	assert(true == success);
	
	success = buffer.tryWrite(44);
	assert(false == success);

	success = buffer.tryRead(value);
	assert (true == success);
	assert (227 == value);

	success = buffer.tryWrite(44);
	assert(true == success);
	assert(trololo[0] = 44);

	success = buffer.tryRead(value);
	assert (42 == value);
	assert(true == success);
	
	success = buffer.tryRead(value);
	assert(true == success);
	assert (43 == value);
	
	success = buffer.tryRead(value);
	assert(true == success);
	assert (44 == value);
	
	success = buffer.tryRead(value);
	assert(false == success);
	
	usart_comm_send_char(value);
	
	#if F_CPU != 16000000UL
	#error Expected F_CPU to be 16MHz; Timer calculation will be wrong!
	#endif

	// Tja nu.
	checkEndianessAndHangUpOnError();

	// USART
	usart_comm_init();
	usart_comm_send_zstr("SYSTEM READY\r\n");
	
	// SPI initialisieren
	spi::SpiMaster spi(&SPI_DDR, &SPI_PORT, &SPI_PIN, SPI_BIT_MOSI, SPI_BIT_MISO, SPI_BIT_SCK, SPI_BIT_SS);
	spi.initialize();
	
	// create an SPI adapter for RFM12
	Rfm12SpiAdapter spiAdapter(&spi);
	
	// Prepare SPI and RFM12
	rfm12_initialize_interrupt();
	
	// Fire and go.
	sei();
		
	// initialisieren des RFM12
	Rfm12 rfm12(&spiAdapter);
			
	// TODO: State machine: Fortfahren, wenn externer Interrupt HIGH.
	using namespace rfm12::commands;
	
	// check interrupt pin
	StatusCommandResult status;
	while ((PIND & (1 << PIND2)) == 0)
	{
		// status read clears interrupt flags in RFM12
		status = rfm12.readStatus();
	}
		
	// signal we're ready to go
	usart_comm_send_zstr("power-on reset cleared\r\n");
	led_doubleflash_sync();
	usart_comm_send_zstr("configuring ...\r\n");
		
	// Set sleep mode
	PowerManagementCommand powerMgmt;
	powerMgmt.setLowBatteryDetectorEnabled(true);
	powerMgmt.setCrystalOscillatorEnabled(false);
	powerMgmt.setClockOutputEnabled(false);
	rfm12.executeCommand(powerMgmt);
	
	// 1000 0000 .... .... Configuration Setting Command
	ConfigSetCommand configSet;
	configSet.setFrequencyBand(FREQ_BAND_433);
	configSet.setCrystalCapacitance(CL_120);
	configSet.setDataRegisterEnabled(true);
	configSet.setFifoEnabled(true);
	rfm12.executeCommand(configSet);
	
	// 1010 .... .... .... Frequency Setting Command
	FrequencyCommand frequencySetting;
	frequencySetting.setByFrequency(433.0F);
	rfm12.executeCommand(frequencySetting);
		
	// 1100 0110 .... .... Data Rate Command
	DataRateCommand dataRate;
	//dataRate.setBitRate(49.2F);
	dataRate.setBitRate(0.6F);
	rfm12.executeCommand(dataRate);

	// 1001 0... .... .... Receiver Control Command
	ReceiverControlCommand receiverControl;
	receiverControl.setPin16Mode(PIN16_VDI_OUT);
	receiverControl.setVdiResponseTime(VDI_FAST);
	receiverControl.setReceiverBasebandBandwidth(RBBW_134);
	receiverControl.setLnaGain(LNGAIN_0);
	receiverControl.setRssiDetectorThreshold(RSSI_91);
	rfm12.executeCommand(receiverControl);
	
	// 1100 0010 .... .... Data Filter Command
	DataFilterCommand dataFilter;
	dataFilter.setClockRecoveryAutoLockModeEnabled(true);
	dataFilter.setClockRecoveryFastLockEnabled(false);
	dataFilter.setDataFilterType(FILTER_DIGITAL);
	dataFilter.setDqdTrheshold(4);
	rfm12.executeCommand(dataFilter);
		
	FifoAndResetModeCommand fifoAndResetMode;
	SynchronPatternCommand synchronPattern;

	uint8_t group = 212; // 212 ist einzige für RFM12 -- sind zwar RFM12B, aber schaden kann es ja nicht
	if (group != 0) {
		// 1100 1010 .... .... FIFO and Reset Mode Command
		fifoAndResetMode.setFifoInterruptFillLevel(8);
		fifoAndResetMode.setSynchronPatternLength(SP_TWO_BYTE);
		fifoAndResetMode.setFifoFillAfterSynchronMatchEnabled(false);
		fifoAndResetMode.setFifoFillStartCondition(FIFOSTART_SYNCHRON);
		fifoAndResetMode.setSensitiveResetMode(RESETMODE_NONSENSITIVE);
		
		// 1100 1110 .... .... Synchron Pattern Command
		synchronPattern.setSynchronByte(group);
	} else {
		// 1100 1010 .... .... FIFO and Reset Mode Command
		fifoAndResetMode.setFifoInterruptFillLevel(8);
		fifoAndResetMode.setSynchronPatternLength(SP_ONE_BYTE);
		fifoAndResetMode.setFifoFillAfterSynchronMatchEnabled(false);
		fifoAndResetMode.setFifoFillStartCondition(FIFOSTART_SYNCHRON);
		fifoAndResetMode.setSensitiveResetMode(RESETMODE_NONSENSITIVE);
		
		// 1100 1110 .... .... Synchron Pattern Command
		synchronPattern.setSynchronByte(0x2D);
	}
	
	rfm12.executeCommand(fifoAndResetMode);
	rfm12.executeCommand(synchronPattern);
	
	// 1100 0100 .... .... AFC Command
	AfcCommand afcCommand;
	afcCommand.setAutomaticOperationMode(AUTOMODE_VDI_HIGH);
	afcCommand.setRangeLimit(MAXDEVIATION_UNRESTRICTED);
	afcCommand.setStrobeEdgeEnabled(false);
	afcCommand.setFineModeEnabled(false);
	afcCommand.setFrequencyOffsetRegisterEnabled(true);
	afcCommand.setFrequencyOffsetCalculationEnabled(true);
	rfm12.executeCommand(afcCommand);
	
	// 1001 100. .... .... TX Configuration Command
	TxConfigCommand txConfig;
	txConfig.setFsk(FSKDF_90KHZ);
	txConfig.setOutputPower(OUTPOW_FULL);
	rfm12.executeCommand(txConfig);
	
	// 1100 1100 0.... .... PLL Setting Command
	PllSettingCommand pllSetting;
	pllSetting.setOutputClockBufferTimeControl(MCCLKFRQ_5OR10MHZ);
	pllSetting.setPhaseDetectorDelayEnabled(false);
	pllSetting.setPllDitheringEnabled(false);
	pllSetting.setPllBandwidth(PLLBW_MAX_2560KBPS);
	rfm12.executeCommand(pllSetting);
	
	// 111. .... .... ....Wake-Up Timer Command
	WakeupTimerCommand wakeupTimer;
	wakeupTimer.disableWakeupTimer();
	rfm12.executeCommand(wakeupTimer);
	
	// 1100 1000 .... .... Low Duty-Cycle Command
	LowDutyCycleCommand lowDutyCycle;
	lowDutyCycle.setEnabled(false);
	rfm12.executeCommand(lowDutyCycle);
	
	// 1100 0000 .... .... Low Battery Detector and Microcontroller Clock Divider Command
	BatteryDetectorAndClockDividerCommand batteryAndClock;
	batteryAndClock.setClockDivider(CLKOUTFREQ_1660kHZ);
	batteryAndClock.setVoltageThreshould(BATTHRESH_3150mV);
	rfm12.executeCommand(batteryAndClock);

	powerMgmt.setReceiverChainEnabled(false);
	powerMgmt.setReceiverBasebandCircuitryEnabled(true);
	powerMgmt.setSynthesizerEnabled(true);
	powerMgmt.setCrystalOscillatorEnabled(true);
	powerMgmt.setTransmissionEnabled(true);
	rfm12.executeCommand(powerMgmt);

	// bye.
	while(1) 
	{
		// sendet jeweils ein Byte -- multiple byte write unterstützt!
		/*
		// synchronize sender and receiver
		rfm12.executeCommandRaw(0b1011100000000000 | (0xAA & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (0xAA & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (0xAA & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (0xAA & 0b0000000011111111));
		
		// synchron pattern
		rfm12.executeCommandRaw(0b1011100000000000 | (0x2D & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (0xD4 & 0b0000000011111111));
		
		// payload
		uint8_t data = 227;
		rfm12.executeCommandRaw(0b1011100000000000 | (data & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (data & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (data & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (data & 0b0000000011111111));
		
		// synchronize sender and receiver
		rfm12.executeCommandRaw(0b1011100000000000 | (0xAA & 0b0000000011111111));
		rfm12.executeCommandRaw(0b1011100000000000 | (0xAA & 0b0000000011111111));
		
		_delay_ms(500);
		*/
		// Register lesen
		// uint16_t values = rfm12.executeCommandRaw(0b1011000000000000);
		
		// enable transmission
		powerMgmt.setTransmissionEnabled(true);
		rfm12.executeCommand(powerMgmt);
		
		// sleep for some time
		for (uint8_t i = 0; i < 10; ++i){
			_delay_ms(100);
		}
		
		// wait until send register is empty
		StatusCommandResult current_status;
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
		
		// transmit a byte
		TransmitRegisterWriteCommand txWrite;
		txWrite.setData(0xAA);
		rfm12.executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
		
		// transmit 0x2D
		txWrite.setData(0x2D);
		rfm12.executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
		
		// transmit 0xD4
		txWrite.setData(0xD4);
		rfm12.executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0x42);
		rfm12.executeCommand(txWrite);
			
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0xB0);
		rfm12.executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0x0B);
		rfm12.executeCommand(txWrite);
			
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0xAA);
		rfm12.executeCommand(txWrite);
			
		// wait until send register is empty
		do { current_status = rfm12.readStatus(); } while (!current_status.rgit_ffit);
		
		// sleep for some time
		for (uint8_t i = 0; i < 10; ++i){
			_delay_ms(100);
		}
		
		// disable transmission
		powerMgmt.setTransmissionEnabled(false);
		rfm12.executeCommand(powerMgmt);
		
		/*
		// output data
		if (current_status.getResultWord() != status.getResultWord())
		{
			status = current_status;
			uint16_t word = status.getResultWord();
		
			usart_comm_send_char(word >> 8);
			usart_comm_send_char(word);
			usart_comm_send_zstr("\r\n");
		}
		*/
		usart_comm_send_zstr("data sent\r\n");
		
		// sleep for some time
		for (uint8_t i = 0; i < 50; ++i){
			_delay_ms(100);
		}
	}
}

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
inline void set_led(switch_t enabled) 
{
	DDRC |= (1 << DDC2);
	
	if (enabled)
		SET_BIT(PORTC, PORTC2);
	else 
		CLEAR_BIT(PORTC, PORTC2);
}


/*! 
	\brief Toggles the LED on and off.
	
	\param ms HIGH duration in milliseconds.
*/
void led_flash_sync()
{
	set_led(ON);
	_delay_ms(100);
	set_led(OFF);
}

/*! 
	\brief Toggles the LED on and off.
*/
void led_doubleflash_sync()
{
	set_led(ON);
	_delay_ms(100);
	set_led(OFF);
	_delay_ms(50);
	set_led(ON);
	_delay_ms(100);
	set_led(OFF);
}