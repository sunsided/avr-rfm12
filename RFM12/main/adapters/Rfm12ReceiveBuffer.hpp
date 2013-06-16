/*
 * Rfm12ReceiveBuffer.hpp
 *
 * Created: 14.06.2013 19:29:12
 *  Author: Markus
 */ 


#ifndef RFM12RECEIVEBUFFER_H_
#define RFM12RECEIVEBUFFER_H_

#include <ringbuffer/RingBuffer.hpp>
#include <rfm12/IReceiveBuffer.hpp>

namespace adapters
{

	/**
	* \brief Adapter that brings Ring Buffers to RFM12.
	*/
	class Rfm12ReceiveBuffer : public rfm12::IReceiveBuffer
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
		inline Rfm12ReceiveBuffer(ringbuffer::RingBuffer *buffer) : _buffer(buffer) {}
		
		/**
		* \brief Unused destructor.
		*/
		inline ~Rfm12ReceiveBuffer() {}
	
		/**
		* \brief Stores a byte in the receive buffer.
		*
		* \param data The byte to store.
		* \return true if the store operation succeeded, false otherwise
		*/
		inline bool store(const uint_least8_t data) {
			return _buffer->tryWrite(data);
		}
		
	
	}; //Rfm12ReceiveBuffer
}

#endif /* RFM12RECEIVEBUFFER_H_ */