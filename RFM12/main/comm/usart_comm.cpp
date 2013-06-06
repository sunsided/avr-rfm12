/*
 * usart_comm.c
 *
 * Created: 14.12.2012 00:28:52
 *  Author: sunside
 */ 

#include <stdlib.h>
#include <assert.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_comm.h"
#include "char_ringbuffer.h"

static uint_least8_t input_buffer_data[USART_INPUT_BUFFER_SIZE];	//!< The input buffer
static char_ringbuffer_t input_buffer;								//!< The input buffer

static uint_least8_t output_buffer_data[USART_OUTPUT_BUFFER_SIZE];	//!< Output buffer data array
static char_ringbuffer_t output_buffer;								//!< The output buffer

static volatile uint_fast8_t output_buffer_free = 0;				//!< Signals if the output buffer is free

/*! 
	\brief Initializes the USART in asynchronous mode
*/
static void init_usart() 
{
	#include <util/setbaud.h>
	
	// UBRR: USART Baud Rate Register
	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;
	
	#if USE_2X
	// UCSRA: USART Control and Status Register A
	UCSR0A |= (1 << U2X0);						// Double USART Transmission Speed
	#else
	UCSR0A &= ~(1 << U2X0);						// Do not double USART Transmission Speed
	#endif
	
	// UCSRB: USART Control and Status Register B
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);      // Enable receiver and transmitter
	UCSR0B |= (1 << RXCIE0);                    // Enable receiver interrupt
	UCSR0B |= (1 << UDRIE0);					// USART Data Register Empty Interrupt Enable
	
	// UCSRC: USART Control and Status Register C
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00);    // Set frame: 8-N-1
}

void usart_comm_init()
{
	// USART initialisieren
	init_usart();
	
	// Empfangs-Ringpuffer initialisieren
	rb_init(&input_buffer, input_buffer_data, USART_INPUT_BUFFER_SIZE); // TODO: Directly assign buffer size - 1 during initialization
	
	// Sende-Ringpuffer initialisieren
	rb_init(&output_buffer, output_buffer_data, USART_OUTPUT_BUFFER_SIZE);
}

void usart_comm_send_char(uint_least8_t data)
{
	rb_put_data_blocking(&output_buffer, data);
	
	// enable the UDRE interrupt
	UCSR0B |= (1 << UDRIE0);
}

void usart_comm_send_zstr(const char* data) 
{
	// burst-write the rest, assuming it's never more than 16 bytes ...
	const char* ptr = data;
	while (0 != *ptr)
	{
		rb_put_data_blocking(&output_buffer, *(ptr++));

		// enable the UDRE interrupt
		UCSR0B |= (1 << UDRIE0);
	}
}

uint_least8_t usart_comm_has_data()
{
	return rb_has_data(&input_buffer);
}

uint_least8_t usart_comm_read_char(uint_least8_t *data)
{
	return rb_get_data(&input_buffer, data);
}

/*!
	\brief Writes a data byte to the USART Communication Interface as binary.
	
	Converts a single byte to binary and prints it as eight chars.
	
	\param data The data byte to write
*/
void usart_comm_send_byte_binary(uint_least8_t data)
{
	usart_comm_send_char(data & 0b10000000 ? '1' : '0');
	usart_comm_send_char(data & 0b01000000 ? '1' : '0');
	usart_comm_send_char(data & 0b00100000 ? '1' : '0');
	usart_comm_send_char(data & 0b00010000 ? '1' : '0');
	usart_comm_send_char(data & 0b00001000 ? '1' : '0');
	usart_comm_send_char(data & 0b00000100 ? '1' : '0');
	usart_comm_send_char(data & 0b00000010 ? '1' : '0');
	usart_comm_send_char(data & 0b00000001 ? '1' : '0');
}

/*! 
	\brief Implements the interrupt service routine for RXC (RX Complete)
	
	This ISR will fetch a byte from the UDR0 input hardware buffer and write it to the software
	receive buffer.
	
	TODO:	When the software receive buffer is almost-full, signal ~CTS, otherwise CTS
*/
ISR (USART_RX_vect)
{
	uint_least8_t receivedChar = UDR0;          // Read data from the RX buffer
	rb_put_data_blocking(&input_buffer, receivedChar);	// Store the data in the software receive buffer
}

/*! 
	\brief Implements the interrupt service routine for UDRE (USART Data Register Empty)
	
	This ISR will take a byte from the output buffer and send it to the UDR0 hardware buffer.
	If no bytes were read, UDRE will be disabled.
	To start sending bytes again, the software will need to enable the UDRE interrupt manually.
	
	TODO:	Theoretically RTS should be honored, but we'll just assume that the endpoint is 
			fast enough to handle our output stream
*/
ISR (USART_UDRE_vect)
{
	uint_least8_t data;
	if (rb_get_data(&output_buffer, &data))
	{	
		// echo the fetched data
		UDR0 = data;
	}
	else
	{
		// disable the UDRE interrupt if there are no more data
		UCSR0B &= ~(1 << UDRIE0);				// USART Data Register Empty Interrupt Disable
	}
}