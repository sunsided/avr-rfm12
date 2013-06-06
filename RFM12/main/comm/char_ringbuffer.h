/*
 * usartdecoder.h
 *
 * Created: 12.12.2012 23:58:51
 *  Author: sunside
 */ 


#ifndef USARTDECODER_H_
#define USARTDECODER_H_

#include <stdint.h>

typedef struct char_ringbuffer {

	uint_least8_t size;						//!< The size of the buffer. Must be a power of two.
	uint_least8_t* data;					//!< The actual data array
	volatile uint_least8_t read_index;		//!< The read index (offset to the data array)
	volatile uint_least8_t write_index;		//!< The write index (offset to the data array)
	volatile uint_least8_t capacity;		//!< The buffer capacity
	
} char_ringbuffer_t;

/*! 
	\brief Initializes a ring buffer.
	
	This method will initialize a ring buffer with the given data array and the given size.
	
	\param buffer	The buffer to initialize
	\param data		The data array to be used for the buffer
	\param size		The size of the data array (and thus of the ring buffer). Must be a power of two.
*/
void rb_init(char_ringbuffer_t* buffer, const uint_least8_t* data, uint_least8_t size);

/*! 
	\brief Resets a ring buffer.
	
	This method will reset a ring buffer
	
	\param buffer	The buffer to reset
*/
void rb_reset(char_ringbuffer_t* buffer);

/*! \brief Determines if the buffer has write capacity
	\return		Zero if there is no write capacity, nonzero otherwise
 */
uint_least8_t rb_get_capacity(const char_ringbuffer_t* buffer);

/*! 
	\brief Determines whether there are data available to read in the buffer
	
	This method will determine if there is data in the buffer that can be read.
	
	\param buffer	The buffer to query
	\return			0 if there is no data, nonzero otherwise
*/
uint_least8_t rb_has_data(const char_ringbuffer_t* buffer);

/*! 
	\brief Puts data in the buffer
	
	This method will store the given data byte in the buffer
	
	\param buffer	The buffer to store the data byte in
	\param data		The data byte to store
*/
void rb_put_data_blocking(char_ringbuffer_t* buffer, uint_least8_t data);

/*! 
	\brief Fetches data from the buffer
	
	This method will read a data byte from the buffer into the given data pointer
	if there is data available.
	
	\param buffer	The buffer to read the data byte from
	\param data		The data byte to store the read byte in
	\return			nonzero if data has been read or 0 if no data was available
*/
uint_least8_t rb_get_data(char_ringbuffer_t* buffer, uint_least8_t *data);

#endif /* USARTDECODER_H_ */