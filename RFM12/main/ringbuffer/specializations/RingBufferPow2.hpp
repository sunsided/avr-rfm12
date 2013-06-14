/* 
* IRingBuffer.h
*
* Created: 14.06.2013 12:40:16
* Author: Markus
*/


#ifndef __IRINGBUFFERPOW2_H__
#define __IRINGBUFFERPOW2_H__

#include <stdint.h>
#include <assert.h>
#include "../RingBuffer.hpp"

namespace ringbuffer
{
	namespace specializations
	{

	class RingBufferPow2 : public RingBuffer
	{
	private:

		/**
		* \brief The size mask
		*
		* This mask is used for the bitwise advance strategy
		*/
		const rbsize_t _sizeMask;	
	
	public:
		/**
		* \brief Initializes the ring buffer.
		*
		* \param backingArray	The array to be used for the buffer
		* \param size			The size of the array
		* \param free_on_delete Sets whether the backing array should be freed during destruction
		*/
		inline RingBufferPow2(rbdata_t *backingArray, const rbsize_t size, bool free_on_delete)
		: RingBuffer(backingArray, size, free_on_delete), _sizeMask(size-1U)
		{
		}

		// virtual ~RingBuffer(){}
		
		/**
		* \brief Tries to write an item to the buffer.
		*
		* If the buffer cannot currently be written to, this operation returns
		* with an appropriate status code.
		*
		* \param item The item to write.
		* \returns true if the write operation succeeded, false otherwise
		*/
		inline bool tryWrite(register const rbdata_t item) {
			// test if the buffer has a free slot
			if (!canWrite()) return false;
		
			// fetch fresh
			_MemoryBarrier();
		
			// since we write, the capacity will be reduced
			--_capacity;
		
			// write to the current write index
			_buffer[_write_index] = item;
		
			// advance the write pointer
			_write_index = (_write_index+1) & this->_sizeMask;
		
			// since we write, the fill level will be increased
			++_fillLevel;
		
			// There we go
			_MemoryBarrier();
			return true;
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

			// fetch fresh
			_MemoryBarrier();	

			// since we read, the fill level will be decreased
			--_fillLevel;
				
			// write to the current write index
			item = _buffer[_read_index];
		
			// advance the read pointer
			_read_index = (_read_index+1) & this->_sizeMask;
		
			// since we read, the capacity will be increased
			++_capacity;
		
			// there we go
			_MemoryBarrier();
			return true;
		}

	}; //IRingBuffer

	}
}

#endif //__IRINGBUFFERPOW2_H__
