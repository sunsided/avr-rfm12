/*
 * main.c
 *
 * Created: 02.06.2013 21:09:43
 *  Author: sunside
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <assert.h>

#include "system.h"
#include <util/delay.h> 

#include "comm/usart_comm.h"
#include "comm/command_decoder.h"

#include "main.h"
#include "spi/SpiMaster.hpp"
#include "rfm12/rfm12.hpp"
#include "ringbuffer/RingBuffer.hpp"

#include "adapters/Rfm12SpiAdapter.hpp"
#include "adapters/Rfm12ReceiveBuffer.hpp"
#include "adapters/Rfm12SendBuffer.hpp"

#include "rfm12configuration.hpp"

using namespace rfm12;

#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_PIN			PINB

#define SPI_BIT_MOSI	PORTB3
#define SPI_BIT_MISO	PINB4
#define SPI_BIT_SCK		PORTB5
#define SPI_BIT_SS		PORTB2

/**
* \brief Determines if a pulse of the rfm12 instance is needed.
*
* This flag is driven by the pin change interrupt handler that processes
* the RFM12's interrupt line.
*/
static volatile bool _rfm12PulseRequired = false;

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

/**
	* \brief Initializes the RFM12 interrupt pin.
	* 
	* \return void
	*/
void rfm12_initialize_interrupt()
{
	// DDRC: Data Direction Register, Port D
	DDRD &= ~(1 << DDD2);		// Pin D2 als Eingang setzen

	// PCICR: Pin Change Interrupt Control Register
	PCICR |= (1 << PCIE2);		// PCMSK2-Überprüfung aktivieren
	
	// PCMSK: Pin Change Mask Register
	PCMSK2 |= (1 << PCINT18);	// Pin D2 triggert interrupt
}

int main()
{
	#if F_CPU != 16000000UL
	#error Expected F_CPU to be 16MHz; Timer calculation will be wrong!
	#endif

	// Tja nu.
	checkEndianessAndHangUpOnError();
	
	led_doubleflash_sync();
	
	// SPI initialisieren
	spi::SpiMaster spi(&SPI_DDR, &SPI_PORT, &SPI_PIN, SPI_BIT_MOSI, SPI_BIT_MISO, SPI_BIT_SCK, SPI_BIT_SS);
	spi.initialize();
	
	// create an SPI adapter for RFM12
	adapters::Rfm12SpiAdapter *rfm12SpiAdapter = new adapters::Rfm12SpiAdapter(&spi);
	
	// create the ring buffers for the RFM12
	ringbuffer::RingBuffer *rfm12ReceiveBuffer = ringbuffer::RingBuffer::create(64);
	adapters::Rfm12ReceiveBuffer *rfm12ReceiveBufferAdapter = new adapters::Rfm12ReceiveBuffer(rfm12ReceiveBuffer);
	
	ringbuffer::RingBuffer *rfm12SendBuffer = ringbuffer::RingBuffer::create(64);
	adapters::Rfm12SendBuffer *rfm12SendBufferAdapter = new adapters::Rfm12SendBuffer(rfm12SendBuffer);
		
	// Prepare SPI and RFM12
	rfm12_initialize_interrupt();
	
	// Fire and go.
	sei();
		
	// USART
	usart_comm_init();
	usart_comm_send_zstr("SYSTEM READY\r\n");
		
	// initialisieren des RFM12
	Rfm12 *rfm12 = new Rfm12(rfm12SpiAdapter, rfm12ReceiveBufferAdapter, rfm12SendBufferAdapter);
			
	// TODO: State machine: Fortfahren, wenn externer Interrupt HIGH.
	using namespace rfm12::commands;
	
	// check interrupt pin
	StatusCommandResult status;
	while ((PIND & (1 << PIND2)) == 0)
	{
		// status read clears interrupt flags in RFM12
		status = rfm12->readStatus();
	}
	
	led_doubleflash_sync();
		
	// signal we're ready to go
	usart_comm_send_zstr("power-on reset cleared\r\n");
	led_doubleflash_sync();
	usart_comm_send_zstr("configuring ...\r\n");
		
	// configure the rfm12
	configureRfm12(rfm12);
	
	// enable transceiver
	PowerManagementCommand powerMgmt; // TODO: Retrieve the current setting from the rfm12 controller!
	powerMgmt.setReceiverChainEnabled(false);
	powerMgmt.setReceiverBasebandCircuitryEnabled(true);
	powerMgmt.setSynthesizerEnabled(true);
	powerMgmt.setCrystalOscillatorEnabled(true);
	powerMgmt.setTransmissionEnabled(true);
	rfm12->executeCommand(powerMgmt);

	// loopity loop.
	for(;;)
	{
		/*
		if (_rfm12PulseRequired) {
			rfm12->pulse();
		}
		*/
		
		// enable transmission
		powerMgmt.setTransmissionEnabled(true);
		rfm12->executeCommand(powerMgmt);
		
		// sleep for some time
		for (uint8_t i = 0; i < 10; ++i){
			_delay_ms(100);
		}
		
		// wait until send register is empty
		StatusCommandResult current_status;
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
		
		// transmit a byte
		TransmitRegisterWriteCommand txWrite;
		txWrite.setData(0xAA);
		rfm12->executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
		
		// transmit 0x2D
		txWrite.setData(0x2D);
		rfm12->executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
		
		// transmit 0xD4
		txWrite.setData(0xD4);
		rfm12->executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0x42);
		rfm12->executeCommand(txWrite);
			
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0xB0);
		rfm12->executeCommand(txWrite);
		
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0x0B);
		rfm12->executeCommand(txWrite);
			
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
			
		// transmit payload
		txWrite.setData(0xAA);
		rfm12->executeCommand(txWrite);
			
		// wait until send register is empty
		do { current_status = rfm12->readStatus(); } while (!current_status.rgit_ffit);
		
		// sleep for some time
		for (uint8_t i = 0; i < 10; ++i){
			_delay_ms(100);
		}
		
		// disable transmission
		powerMgmt.setTransmissionEnabled(false);
		rfm12->executeCommand(powerMgmt);
		


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
	
	_delay_ms(100);
	_delay_ms(100);
	_delay_ms(100);
}

/*! 
	\brief Implements the interrupt service routine for INT0 (e.g. a pin level change)
*/
ISR (PCINT2_vect)
{
	static uint_least8_t last_value = 0;
	uint_least8_t current_value = PIND & (1 << PIND2);
	
	// RFM12 INT ist active low
	if (current_value < last_value) 
	{
		_rfm12PulseRequired = true;
		set_led(ON);
	}
	
	last_value = current_value;
}	