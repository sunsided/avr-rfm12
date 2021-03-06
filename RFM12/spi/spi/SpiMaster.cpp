/*
 * Spi.cpp
 *
 * Created: 11.06.2013 02:28:17
 *  Author: Markus
 */ 

#include <avr/io.h>
#include "SpiMaster.hpp"

namespace spi
{
	/**
	* \brief Initializes the SPI.
	*/
	void SpiMaster::initialize() const
	{
		// Set MOSI, SCK and SS output, MISO input
		*_ddr_register |= ((1 << _mosi_bit)
					   |  (1 << _sck_bit) 
					   |  (1 << _ss_bit))
					   & ~(1 << _miso_bit);
		
		// Configure SPI
		SPCR = ((1 << SPE)	// SPI enabled
			| (1 << MSTR)	// SPI Master mode
			| (0 << SPR1)	// OSC/16 --> 1MHz SPI at 16 MHz CPU_FREQ
			| (1 << SPR0))  //  --"--
			& ~(1 << DORD)  // MSB first
			& ~(1 << CPHA)	// Polarity and phase set to default (mode 1)
			& ~(1 << CPOL); //  --"--
			
		// TODO: Interrupt-driven operation, SPIE	
		// NOTE: RFM12 uses 16bit SPI!
		
		// Simple SPI rate --> fsck/64
		SPSR &= ~(1 << SPI2X);
		
		// Activate pull ups for SS
		*_input_register |= (1 << _ss_bit);
		
		// Actively pull high SS
		endTransmission();
	}
	
	/**
	* \brief Transmits a byte of data.
	*
	* This method sends data synchronously by blocking on the
	* SPI send register.
	*
	* \param data The data to send.
	* \returns The result.
	*/
	uint_fast8_t SpiMaster::transmitSync(uint_fast8_t data) const
	{
		// Start transmission
		SPDR = (uint8_t)data;
		
		// Wait for transmission complete
		while(!(SPSR & (1 << SPIF)));
		
		// Output read data
		return SPDR;
	}
	
	/**
	* \brief Transmits two bytes of data.
	*
	* This method sends data synchronously by blocking on the
	* SPI send register.
	*
	* \param data The data to send.
	* \returns The result.
	*/
	uint_fast16_t SpiMaster::transmitSync(uint_fast16_t data) const
	{
		uint16_t result;
		
		// Transmit first byte
		SPDR = (uint8_t)(data >> 8) & 0xFF;
		while(!(SPSR & (1 << SPIF)));
		result = SPDR << 8;
		
		// Transmit second byte
		SPDR = (uint8_t)data & 0xFF;
		while(!(SPSR & (1 << SPIF)));
		result |= SPDR;
		
		// And off we go.
		return result;
	}
}