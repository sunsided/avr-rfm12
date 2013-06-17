/*
 * usart_comm.h
 *
 * Created: 14.12.2012 00:09:18
 *  Author: sunside
 */ 


#ifndef USART_COMM_H_
#define USART_COMM_H_

#include "../system.h"
#include <stdint.h>

#ifndef BAUD
	#define BAUD 115200L								// 250k, 500k und 1M haben alle einen Fehler von 0%!
	//#define BAUD 250000L
#endif

#ifndef BAUD_TOL
	#define BAUD_TOL 3									//!< Baud error tolerance in percent
#endif

#define USART_INPUT_BUFFER_SIZE (32)					//!< Size of the input buffer
#define USART_OUTPUT_BUFFER_SIZE (64)					//!< Size of the output buffer

#define CHAR_NUL (0x00)									//!< NUL character
#define CHAR_STX (0x02)									//!< Start-of-Text character
#define CHAR_ETX (0x03)									//!< End-of-Text character

/*!
	\brief Initializes the USART Communication Interface
	
	\param input_buffer		The input buffer
	\param output_buffer	The output buffer
*/
void usart_comm_init();

/*!
	\brief Writes a data byte to the USART Communication Interface
	
	\param data The data byte to write
*/
void usart_comm_send_char(uint_least8_t data);

/*!
	\brief Writes a data word to the USART Communication Interface
	
	\param data The data word to write
*/
inline void usart_comm_send_word(const uint_least16_t data) {
	usart_comm_send_char((data >> 8) & 0xFF);
	usart_comm_send_char(data        & 0xFF);
}

/*!
	\brief Writes a data byte to the USART Communication Interface as binary.
	
	Converts a single byte to binary and prints it as eight chars.
	
	\param data The data byte to write
*/
void usart_comm_send_byte_binary(uint_least8_t data);

/*!
	\brief Writes a zero terminated data stream to the USART Communication Interface
	
	\param data The data byte to write
*/
void usart_comm_send_zstr(const char* data);

/*!
	\brief Determines whether the USART Communication Interface has data available to read
	
	\return		Nonzero if a byte has been read, 0 if no byte was read
*/
uint_least8_t usart_comm_has_data();

/*!
	\brief Reads a data byte from the USART Communication Interface
	
	\param data The data byte to read into
	\return		Nonzero if a byte has been read, 0 if no byte was read
*/
uint_least8_t usart_comm_read_char(uint_least8_t *data);



#endif /* USART_COMM_H_ */