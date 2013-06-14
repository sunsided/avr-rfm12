/* 
* IRingBuffer.h
*
* Created: 14.06.2013 12:40:16
* Author: Markus
*/


#ifndef __IRINGBUFFER_H__
#define __IRINGBUFFER_H__

#include <stdint.h>
#include <assert.h>

/**
* \brief Ring buffer data type
*/
typedef uint_least8_t rbdata_t;

/**
* \brief Ring buffer size type
*/
typedef uint_least8_t rbsize_t;

class RingBuffer
{
private:
	/**
	* \brief The actual data buffer
	*/
	rbdata_t volatile *_buffer;

	/**
	* \brief The actual size of the ring buffer
	*/
	const rbsize_t _size;
	
	/**
	* \brief The size mask
	*
	* This mask is used for the bitwise advance strategy
	*/
	const rbsize_t _sizeMask;
		
	/**
	* \brief The buffer's capacity, i.e. free space
	*/
	volatile rbsize_t _capacity;
	
	/**
	* \brief The buffer's fill level, i.e. used space
	*/
	volatile rbsize_t _fillLevel;
		
	/**
	* \brief The current read index (in the _buffer array)
	*/
	volatile rbsize_t _read_index;
	
	/**
	* \brief The current write index (in the _buffer array)
	*/
	volatile rbsize_t _write_index;
			
public:
	/**
	* \brief Initializes the ring buffer.
	*
	* \param backingArray	The array to be used for the buffer
	* \param size			The size of the array
	*/
	inline RingBuffer(rbdata_t *backingArray, const rbsize_t size)
	:	_buffer(backingArray), _size(size), _sizeMask(size-1U), 
		_capacity(size), _fillLevel(0), _read_index(0), _write_index(0)
	{
		assert(backingArray != NULL);
		assert(size > 0);
		
		// TODO: Check the capacity!
		
		// select the buffer strategy
		bool sizeIsPowerOfTwo = ((size & (size - 1)) == 0);
		if (sizeIsPowerOfTwo) {
			// select bitwise strategy
			advance = &RingBuffer::advanceBitwise;
		}
		else {
			// select comparison strategy
			advance = &RingBuffer::advanceConditional;
		}
		
		// prepare the buffer
		#ifdef DEBUG
		for (rbsize_t i=0; i<size; ++i)
		{
			_buffer[i] = '#';
		}	
		#endif
	}

	// virtual ~RingBuffer(){}
		
	/**
	* \brief Determines if an item can be read from this buffer.
	*/
	inline bool canRead() const { 
		return _fillLevel > 0;
	}
	
	/**
	* \brief Determines if an item can be read written to this buffer.
	*/
	inline bool canWrite() const { 
		return _capacity > 0;
	}

	/**
	* \brief Tries to write an item to the buffer.
	*
	* If the buffer cannot currently be written to, this operation returns
	* with an appropriate status code.
	*
	* \param item The item to write.
	* \returns true if the write operation succeeded, false otherwise
	*/
	inline bool tryWrite(register rbdata_t item) {
		// test if the buffer has a free slot
		if (!canWrite()) return false;
		
		// since we write, the capacity will be reduced
		--_capacity;
		
		// write to the current write index
		_buffer[_write_index] = item;
		
		// advance the write pointer
		(this->*this->advance)(&_write_index);
		
		// since we write, the fill level will be increased
		++_fillLevel;
		
		return true;
	}

	/**
	* \brief Writes an item to the buffer.
	*
	* If the buffer cannot currently be written to, this operation blocks.
	*
	* \param item The item to write.
	*/
	inline void writeSync(rbdata_t item) {
		// loop until the buffer has a free slot
		while (!tryWrite(item)) ;
	}
	
	/**
	* \brief Tries to reads an item from the buffer.
	*
	* If the buffer cannot currently be read from, this operation returns
	* with an appropriate status code.
	*
	* \param item out: The read item. If the return code is false, the 
	*				value is undefined.
	* \returns true if an item was read, false otherwise
	*/
	inline bool tryRead(register rbdata_t &item) { 
		// loop until the buffer has a full slot
		if (!canRead()) return false;

		// since we read, the fill level will be decreased
		--_fillLevel;
				
		// write to the current write index
		item = _buffer[_read_index];
		
		// advance the write pointer
		(this->*this->advance)(&_read_index);
		
		// since we read, the capacity will be increased
		++_capacity;
		
		// there we go
		return true;
	}
	
	/**
	* \brief Reads an item from the buffer.
	*
	* If the buffer cannot currently be read from, this operation blocks.
	*
	* \returns The read item.
	*/
	inline rbdata_t readSync() { 	
		// loop until the buffer has a full slot
		rbdata_t item;
		while (!tryRead(item)) ;
		
		// there we go
		return item;
	}

private:
	/**
	* \brief Pointer to the selected advance method
	*/
	void (RingBuffer::*advance)(register rbsize_t volatile *pointer);

	/**
	* \brief Bitwise-and advance strategy
	*/
	inline void advanceBitwise(register rbsize_t volatile *pointer) {
		*pointer = (*pointer+1) & this->_sizeMask;
	}
	
	/**
	* \brief Conditional advance strategy
	*/
	inline void advanceConditional(register rbsize_t volatile *pointer) {
		++*pointer;
		if (*pointer == this->_size) {
			*pointer = 0;			
		}
	}

}; //IRingBuffer

#endif //__IRINGBUFFER_H__
