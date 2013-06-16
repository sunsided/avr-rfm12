/* 
* IRingBuffer.h
*
* Created: 14.06.2013 12:40:16
* Author: Markus
*/


#ifndef __IRINGBUFFERANYSIZE_H__
#define __IRINGBUFFERANYSIZE_H__

#include <stdint.h>
#include <assert.h>
#include "../RingBuffer.hpp"

/**
* \brief Instruction for condition prediction
*/
#ifndef likely_match(x)
#ifdef __GNUC__
#define likely_match(x)		__builtin_expect(!!(x), 1)		//<! GCC specific, http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html#index-g_t_005f_005fbuiltin_005fexpect-3067
#else
#define likely_match(x)		(x)
#endif /* __GNUC__ */
#endif /* likely_match */

/**
* \brief Instruction for condition prediction
*/
#ifndef unlikely_match(x)
#ifdef __GNUC__
#define unlikely_match(x)	__builtin_expect(!!(x), 0)		//<! GCC specific, http://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html#index-g_t_005f_005fbuiltin_005fexpect-3067
#else
#define unlikely_match(x)	(x)
#endif /* __GNUC__ */
#endif /* unlikely_match */

namespace ringbuffer 
{
	namespace specializations 
	{
		class RingBufferAnySize : public RingBuffer
		{
	
		public:
			/**
			* \brief Initializes the ring buffer.
			*
			* \param backingArray	The array to be used for the buffer
			* \param size			The size of the array
			* \param free_on_delete Sets whether the backing array should be freed during destruction
			*/
			inline RingBufferAnySize(rbdata_t *backingArray, const rbsize_t size, bool free_on_delete)
			:	RingBuffer(backingArray, size, free_on_delete)
			{
				assert(size < (2^sizeof(rbsize_t)));
				
				// this implementation is based on the idea that by marching through
				// memory backwards (i.e. decreasing the pointers before reading) only 
				// an conditional jump based on zero is required, which should increase performance.
				_write_index = _size;
				_read_index = _size;
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
		
				// write to the current write index and advance the pointer
				_buffer[--_write_index] = item;
		
				// reset the write pointer if needed
				if (unlikely_match(0 == _write_index)) {
					_write_index = _size;
				}
		
				// since we write, the fill level will be increased
				++_fillLevel;
		
				// there we go
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
				
				// write to the current write index and advance the pointer
				item = _buffer[--_read_index];
		
				// reset the read pointer if needed
				if (unlikely_match(0 == _read_index)) {
					_read_index = _size;
				}
		
				// since we read, the capacity will be increased
				++_capacity;
		
				// there we go
				_MemoryBarrier();
				return true;
			}


		}; //IRingBuffer

	}
}
#endif //__IRINGBUFFERANYSIZE_H__
