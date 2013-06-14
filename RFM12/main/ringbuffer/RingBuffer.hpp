/* 
* IRingBuffer.h
*
* Created: 14.06.2013 12:40:16
* Author: Markus
*/


#ifndef __RINGBUFFER_H__
#define __RINGBUFFER_H__

#include <stdint.h>

// #include <avr/cpufunc.h> 
#ifndef _MemoryBarrier
#define _MemoryBarrier() __asm__ __volatile__("":::"memory")
#endif

namespace ringbuffer 
{

	/**
	* \brief Ring buffer data type
	*/
	typedef uint_least8_t rbdata_t;

	/**
	* \brief Ring buffer size type
	*/
	typedef uint_least8_t rbsize_t;

	/**
	* \brief Ring buffer base class and factory.
	*/
	class RingBuffer
	{
	public:
		/**
		* Creates a new ring buffer instance
		*
		* An optimized specialization will be instanciated if the buffer size is a power of two.
		*
		* \param backingArray	The array to be used for the buffer
		* \param size			The size of the array
		* \param free_on_delete Sets whether the backing array should be freed during destruction
		*/
		static RingBuffer* create(rbdata_t *backingArray, const rbsize_t size, bool free_on_delete = false);

		/**
		* Creates a new ring buffer instance
		*
		* An optimized specialization will be instanciated if the buffer size is a power of two.
		*
		* \param size			The size of the array
		*/
		static RingBuffer* create(const rbsize_t size);

	protected:
		/**
		* \brief The actual data buffer
		*/
		rbdata_t *_buffer;

		/**
		* \brief The actual size of the ring buffer
		*/
		const rbsize_t _size;
			
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
		rbsize_t _read_index;
	
		/**
		* \brief The current write index (in the _buffer array)
		*/
		rbsize_t _write_index;

		/**
		* \brief Sets whether the buffer should be freed during instance destruction
		*/
		bool _free_on_delete;

	protected:

		/**
		* \brief Initializes the ring buffer.
		*
		* \param backingArray	The array to be used for the buffer
		* \param size			The size of the array
		* \param free_on_delete Sets whether the backing array should be freed during destruction
		*/
		RingBuffer(rbdata_t *backingArray, const rbsize_t size, bool free_on_delete);

	public:
	
		/**
		* \brief Destructs in instance
		*/
		virtual ~RingBuffer();
		
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
		virtual bool tryWrite(register const rbdata_t item) = 0;

		/**
		* \brief Writes an item to the buffer.
		*
		* If the buffer cannot currently be written to, this operation blocks.
		*
		* \param item The item to write.
		*/
		inline void writeSync(register const rbdata_t item) {
			// loop until the buffer has a free slot
			do {} while (!tryWrite(item)) ;
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
		virtual bool tryRead(register rbdata_t &item) = 0;
	
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
			do {} while (!tryRead(item));
		
			// there we go
			return item;
		}

	}; //RingBuffer

}

#endif //__RINGBUFFER_H__
