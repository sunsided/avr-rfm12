/* 
* Rfm12SpiAdapter.h
*
* Created: 11.06.2013 01:52:18
* Author: Markus
*/


#ifndef __RFM12SPIADAPTER_H__
#define __RFM12SPIADAPTER_H__

#include "spi/SpiMaster.hpp"
#include "rfm12/ISpi.hpp"

/**
* \brief Adapter that brings SPI to RFM12.
*/
class Rfm12SpiAdapter : public rfm12::ISpi
{
private:
	/**
	* \brief The SPI instance.
	*/
	const spi::SpiMaster *_spi;
	
public:
	/**
	* \brief Initializes this instance.
	*
	* \param spi The SPI interface
	*/
	inline Rfm12SpiAdapter(const spi::SpiMaster *spi) : _spi(spi) {}
		
	/**
	* \brief Unused destructor.
	*/
	inline ~Rfm12SpiAdapter() {}
	
	/**
	* \brief Begins a transmission by pulling Slave Select low.
	*/
	inline void beginTransmission() const
	{
		_spi->beginTransmission();
	}
	
	/**
	* \brief Ends a transmission by pulling Slave Select high.
	*/
	inline void endTransmission() const
	{
		_spi->endTransmission();
	}

	/**
	* \brief Transmits two bytes of data.
	*
	* \param data The data to send.
	* \returns The result.
	*/
	inline const uint_fast16_t transmitWord(uint_fast16_t data) const
	{
		return _spi->transmitSync(data);
	}
		
private:
	/**
	* \brief Copy constructor.
	*/
	Rfm12SpiAdapter( const Rfm12SpiAdapter &c ) : _spi(c._spi) {}
		
	/**
	* \brief Copy operator.
	*/		
	Rfm12SpiAdapter& operator=( const Rfm12SpiAdapter &c ) { this->_spi = c._spi; return *this; }

}; //Rfm12SpiAdapter

#endif //__RFM12SPIADAPTER_H__
