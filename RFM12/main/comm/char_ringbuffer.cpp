/*
 * char_ringbuffer.c
 *
 * Created: 13.12.2012 00:17:22
 *  Author: sunside
 */ 

#include <stdlib.h>
#include <assert.h>
#include <util/atomic.h>
#include "char_ringbuffer.h"

void rb_init(char_ringbuffer_t* buffer, const uint_least8_t* data, uint_least8_t size)
{
	assert(buffer != 0);
	assert(data != 0);
	assert(size > 0);
	
	buffer->size = size;
	buffer->data = const_cast<uint_least8_t*>(data);
	rb_reset(buffer);
}

void rb_reset(char_ringbuffer_t* buffer) 
{
	buffer->read_index = 0;
	buffer->write_index = 0;
	buffer->capacity = buffer->size-1;

	// TODO: Debug only
	for (uint_least8_t i=0; i<buffer->size; ++i)
	{
		buffer->data[i] = '#';
	}
}

uint_least8_t rb_has_data(const char_ringbuffer_t* buffer)
{
	assert(buffer != 0);
	return buffer->read_index != buffer->write_index;
}

uint_least8_t rb_get_capacity(const char_ringbuffer_t* buffer)
{
	uint_least8_t capacity;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		capacity = buffer->capacity;
	}
	return capacity;
}

void rb_put_data_blocking(char_ringbuffer_t* buffer, uint_least8_t data)
{
	assert(buffer != 0);
	
	// TODO: implement non-blocking using return value?
	while (0 == rb_get_capacity(buffer));

	// data will be written, so reduce the capacity
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		--buffer->capacity;
	}

	// write the data
	buffer->data[buffer->write_index] = data;
	
	// advance write pointer
	// assuming that CHAR_RINGBUFFER_SIZE is a power of 2, the and operation
	// against CHAR_RINGBUFFER_SIZE-1 equals a modulo CHAR_RINGBUFFER_SIZE
	buffer->write_index = (buffer->write_index+1) & (buffer->size-1U);
}

uint_least8_t rb_get_data(char_ringbuffer_t* buffer, uint_least8_t *data)
{
	assert(buffer != 0);
	assert(data != 0);
	
	if (buffer->read_index != buffer->write_index)
	{
		*data = buffer->data[buffer->read_index];
	
		// advance read pointer
		// assuming that CHAR_RINGBUFFER_SIZE is a power of 2, the and operation
		// against CHAR_RINGBUFFER_SIZE-1 equals a modulo CHAR_RINGBUFFER_SIZE
		buffer->read_index = (buffer->read_index+1) & (buffer->size-1U);
	
		// data has been read, so increase the capacity
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
		{
			++buffer->capacity;
		}
	
		// signal that data was read
		return 1;
	}
	
	// signal that no data was read
	return 0;
}