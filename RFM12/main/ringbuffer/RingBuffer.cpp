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

RingBuffer::RingBuffer(rbdata_t *backingArray, const rbsize_t size)
:	_buffer(backingArray), _size(size),
_capacity(size), _fillLevel(0), _read_index(0), _write_index(0)
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

RingBuffer* RingBuffer::create(rbdata_t *backingArray, const rbsize_t size) {
	// select the buffer strategy
	bool sizeIsPowerOfTwo = ((size & (size - 1)) == 0);
	
	if (sizeIsPowerOfTwo) {
		// select bitwise strategy
		return new RingBufferPow2(backingArray, size);
	}
	else {
		return new RingBufferAnySize(backingArray, size);
	}
}