/*
 * ISendBuffer.hpp
 *
 * Created: 14.06.2013 17:09:39
 *  Author: Markus
 */ 


#ifndef ISENDBUFFER_H_
#define ISENDBUFFER_H_

namespace rfm12
{

	/**
	* \brief Interface to the send buffer
	*/
	class ISendBuffer 
	{
	public:	
		/**
		* Virtual destructor
		*/
		inline virtual ~ISendBuffer(){}

		/**
		* \brief Fetches a byte form the send buffer
		*
		* \param data out - The byte to be sent. If the return value is false,
		*				this value is undefined.
		* \return true if a byte was fetched, false otherwise
		*/
		virtual bool fetch(uint_least8_t *data) = 0;
	};

}

#endif /* ISENDBUFFER_H_ */