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

#include "leds.hpp"
#include "adapters/Rfm12SpiAdapter.hpp"
#include "adapters/Rfm12ReceiveBuffer.hpp"
#include "adapters/Rfm12SendBuffer.hpp"

#include "rfm12configuration.hpp"
#include "demoloops.hpp"
#include "main.hpp"

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
volatile bool _rfm12PulseRequired = false;

int main()
{
	#if F_CPU != 16000000UL
	#error Expected F_CPU to be 16MHz; Timer calculation will be wrong!
	#endif
	
	wdt_disable();
	
	led_flash_sync();
	red_sleep(5);
	led_flash_sync();
	
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
	
	uint8_t group = 212;
	configureRfm12(rfm12, group);
	
	led_doubleflash_sync();
	sleep(1);
	
	// sanity checking
#ifdef DEBUG
	LED_ASSERT(rfm12->getPowerManagementCommand()->checkBitfieldIsValid(), 10);
#endif	
		
	usart_comm_send_zstr("transmitter configured.\r\n");
	sleep(1);
	
#if DEMO_TRANSMITTER_MODE
	transmitterDemoLoop(rfm12, rfm12SendBuffer, group);
#else
	receiverDemoLoop(rfm12, rfm12ReceiveBuffer);
#endif

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
