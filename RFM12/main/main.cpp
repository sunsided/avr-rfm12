/*
 * main.c
 *
 * Created: 02.06.2013 21:09:43
 *  Author: sunside
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>
#include <avr/wdt.h>

#include <ringbuffer/RingBuffer.hpp>
#include <rfm12/Rfm12.hpp>
#include <spi/SpiMaster.hpp>

#include "comm/usart_comm.h"
#include "comm/command_decoder.h"

#include "main.h"
#include "adapters/Rfm12SpiAdapter.hpp"
#include "adapters/Rfm12ReceiveBuffer.hpp"
#include "adapters/Rfm12SendBuffer.hpp"

#include "rfm12configuration.hpp"

using namespace rfm12;
using namespace rfm12::commands;

#define SPI_DDR				DDRB
#define SPI_PORT			PORTB
#define SPI_PIN				PINB

#define SPI_BIT_MOSI		PORTB3
#define SPI_BIT_MISO		PINB4
#define SPI_BIT_SCK			PORTB5
#define SPI_BIT_SS			PORTB2

/**
* \brief Determines if a pulse of the rfm12 instance is needed.
*
* This flag is driven by the pin change interrupt handler that processes
* the RFM12's interrupt line.
*/
static volatile bool _rfm12PulseRequired = false;

/**
* \brief State of the send demo
*/
static enum {
	TXSTATE_IDLE_TXOFF,			//<! System is idle and transmitter is disabled
	TXSTATE_IDLE_TXON,			//<! System is idle and transmitter is enabled
	TXSTATE_TRANSMITTING,		//<! System is transmitting
} txdemostate = TXSTATE_IDLE_TXOFF;

int main()
{
	wdt_disable();
	
	sleep(5);
	led_doubleflash_sync();
	
	#if F_CPU != 16000000UL
	#error Expected F_CPU to be 16MHz; Timer calculation will be wrong!
	#endif
	
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
	initializeRfm12Interrupt();
	
	// Fire and go.
	sei();
		
	// USART
	usart_comm_init();
	usart_comm_send_zstr("SYSTEM READY\r\n");
		
	// initialisieren des RFM12
	Rfm12 *rfm12 = new Rfm12(rfm12SpiAdapter, rfm12ReceiveBufferAdapter, rfm12SendBufferAdapter);
		
	// configure the rfm12
	usart_comm_send_zstr("configuring ...\r\n");
	configureRfm12(rfm12);
	
	led_doubleflash_sync();
	sleep(1);
	
	// sanity checking
#ifdef DEBUG
	LED_ASSERT(rfm12->getPowerManagementCommand()->checkBitfieldIsValid(), 10);
#endif	
	
	// enable fast-switching strategy and commit (receiver still in idle mode)
	rfm12->setTransceiverStrategy(RXTXSTRATEGY_FAST_TRANSMITTER, true);
	
	usart_comm_send_zstr("transmitter configured.\r\n");
	sleep(1);

	// loopity loop.
	for(;;)
	{
		switch (txdemostate) 
		{
			case TXSTATE_IDLE_TXOFF: 
			{
				// at this point, the receiver should not be active, so we may very well reset the buffer
				rfm12SendBuffer->reset();
				rfm12SendBuffer->writeSync(0xAA);
				rfm12SendBuffer->writeSync(0xAA);
				rfm12SendBuffer->writeSync(0xAA);
				rfm12SendBuffer->writeSync(0xAA);
				
				rfm12SendBuffer->writeSync(0x2D);
				rfm12SendBuffer->writeSync(0xD4);
				
				rfm12SendBuffer->writeSync(0x01);
				rfm12SendBuffer->writeSync(0x02);
				rfm12SendBuffer->writeSync(0x03);
				rfm12SendBuffer->writeSync(0x04);
				rfm12SendBuffer->writeSync(0x42);
				rfm12SendBuffer->writeSync(0xB0);
				rfm12SendBuffer->writeSync(0x0B);
				rfm12SendBuffer->writeSync(0xE3);
				
				rfm12SendBuffer->writeSync(0xAA);
								
				// enable transmitter, then sleep
				rfm12->enterTransmitterMode();
				usart_comm_send_zstr("transmitter on ...\r\n");
				// sleep(1);
				
				txdemostate = TXSTATE_TRANSMITTING;
				break;
			}
						
			case TXSTATE_TRANSMITTING:
			{			
				// the interupt knows best
				if (_rfm12PulseRequired) {
					rfm12->pulse();
					_rfm12PulseRequired = false;
				}
				
				// if the transmission is not done, do not switch state
				if (!rfm12->isTransmissionDone()) break;
				usart_comm_send_zstr("data sent.\r\n");
				txdemostate = TXSTATE_IDLE_TXON;
				break;
			}
			
			case TXSTATE_IDLE_TXON:
			{
				// sleep(1);
				
				// disable transmission
				rfm12->enterIdleMode();
				usart_comm_send_zstr("transmitter off.\r\n");
				
				// sleep for some time
				sleep(5);
				txdemostate = TXSTATE_IDLE_TXOFF;
				break;
			}
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
	DDRC |= (1 << DDC1);
	
	if (enabled) {
		SET_BIT(PORTC, PORTC2);
	}
	else {
		CLEAR_BIT(PORTC, PORTC2);
	}
}

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
inline void set_red_led(switch_t enabled) 
{
	DDRC |= (1 << DDC1);
	
	if (enabled) {
		SET_BIT(PORTC, PORTC1);
	}
	else {
		CLEAR_BIT(PORTC, PORTC1);
	}
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
	uint_least8_t current_value = RFM12_INTERRUPT_PIN_VALUE;
	
	// RFM12 INT ist active low
	if (current_value < last_value) 
	{
		_rfm12PulseRequired = true;
		set_led(ON);
	}
	
	last_value = current_value;
}	