/*
 * RingBuffer.cpp
 *
 * Created: 14.06.2013 15:43:00
 *  Author: Markus
 */ 

#include <stdlib.h>
#include <assert.h>

#include "RingBuffer.hpp"
#include "specializations/RingBufferAnySize.hpp"
#include "specializations/RingBufferPow2.hpp"

RingBuffer::RingBuffer(rbdata_t *backingArray, const rbsize_t size, bool free_on_delete)
:	_buffer(backingArray), _size(size), _capacity(size), _fillLevel(0), _read_index(0), _write_index(0), _free_on_delete(free_on_delete)
{
	assert(backingArray != NULL);
	assert(size > 0);
	
	// prepare the buffer
	#ifdef DEBUG
	for (rbsize_t i=0; i<size; ++i)
	{
		_buffer[i] = '#';
	}
	#endif
}

/**
* \brief Destructs in instance
*/
RingBuffer::~RingBuffer() 
{
	if (this->_free_on_delete)	{
		void* buffer = _buffer;
		free(buffer);
	}
}

/**
* Creates a new ring buffer instance
*
* \param backingArray	The array to be used for the buffer
* \param size			The size of the array
* \param free_on_delete Sets whether the backing array should be freed during destruction
*/
RingBuffer* RingBuffer::create(rbdata_t *backingArray, const rbsize_t size, bool free_on_delete) {
	// select the buffer strategy
	bool sizeIsPowerOfTwo = ((size & (size - 1)) == 0);
	
	if (sizeIsPowerOfTwo) {
		// select bitwise strategy
		return new RingBufferPow2(backingArray, size, free_on_delete);
	}
	else {
		return new RingBufferAnySize(backingArray, size, free_on_delete);
	}
}

/**
* Creates a new ring buffer instance
*
* \param size			The size of the array
*/
RingBuffer* RingBuffer::create(const rbsize_t size) {
	rbdata_t *buffer = static_cast<rbdata_t *>(malloc(size * sizeof(rbsize_t)));
	return create(buffer, size, true);
}