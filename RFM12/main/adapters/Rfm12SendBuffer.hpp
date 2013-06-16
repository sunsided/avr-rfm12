/*
 * Rfm12SendBuffer.hpp
 *
 * Created: 14.06.2013 19:36:47
 *  Author: Markus
 */ 


#ifndef RFM12SENDBUFFER_H_
#define RFM12SENDBUFFER_H_


#include <ringbuffer/RingBuffer.hpp>
#include <rfm12/ISendBuffer.hpp>

namespace adapters
{

	/**
	* \brief Adapter that brings Ring Buffers to RFM12.
	*/
	class Rfm12SendBuffer : public rfm12::ISendBuffer
	{
	private:
		/**
		* \brief The Ring Buffer instance.
		*/
		ringbuffer::RingBuffer *_buffer;
	
	public:
		/**
		* \brief Initializes this instance.
		*
		* \param spi The SPI interface
		*/
		inline Rfm12SendBuffer(ringbuffer::RingBuffer *buffer) : _buffer(buffer) {}
		
		/**
		* \brief Unused destructor.
		*/
		inline ~Rfm12SendBuffer() {}
			
		/**
		* \brief Fetches a byte form the send buffer
		*
		* \param data out - The byte to be sent. If the return value is false,
		*				this value is undefined.
		* \return true if a byte was fetched, false otherwise
		*/
		inline bool fetch(uint_least8_t *data) {
			return _buffer->tryRead(data);
		}
	
	}; //Rfm12SendBuffer
}

#endif /* RFM12SENDBUFFER_H_ */