/* 
* IReceiveBuffer.h
*
* Created: 14.06.2013 17:14:16
* Author: Markus
*/


#ifndef __IRECEIVEBUFFER_H__
#define __IRECEIVEBUFFER_H__

namespace rfm12
{

	/**
	* \brief Interface to the receive buffer
	*/
	class IReceiveBuffer
	{
	public:
		/**
		* Virtual destructor
		*/
		inline virtual ~IReceiveBuffer(){}

		/**
		* \brief Stores a byte in the receive buffer.
		*
		* \param data The byte to store.
		* \return true if the store operation succeeded, false otherwise
		*/
		virtual bool store(const uint_least8_t data) = 0;

	}; //IReceiveBuffer

}

#endif //__IRECEIVEBUFFER_H__
