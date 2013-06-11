/* 
* ISpi.h
*
* Created: 11.06.2013 01:37:08
* Author: Markus
*/


#ifndef __ISPI_H__
#define __ISPI_H__

#include <stdint.h>

namespace rfm12
{
	/**
	* \brief Interface for SPI related operations.
	*/
	class ISpi
	{
	public:
		// inline virtual ~ISpi(){}
			
		/**
		* \brief Begins a transmission by pulling Slave Select low.
		*/
		virtual void beginTransmission() const = 0;
		
		/**
		* \brief Ends a transmission by pulling Slave Select high.
		*/
		virtual void endTransmission() const = 0;

		/**
		* \brief Transmits two bytes of data.
		*
		* \param data The data to send.
		* \returns The result.
		*/
		virtual const uint_fast16_t transmitWord(uint_fast16_t data) const = 0;

	}; //ISpi

}

#endif //__ISPI_H__
