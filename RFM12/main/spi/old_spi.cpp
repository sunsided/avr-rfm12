/*
 * spi.c
 *
 * Created: 02.06.2013 21:22:14
 *  Author: sunside
 */ 

#include <avr/io.h>
#include "old_spi.h"

/**
 * \brief Initializes the SPI interface.
 * 
 * \return void
 */
void spi_initialize()
{
	// Set MOSI, SCK and SS output
	SPI_DDR |= (1<<SPI_DD_MOSI)| (1<<SPI_DD_SCK) | (1<<SPI_DD_SS);
		
	// Enable SPI, Master, set clock rate fck/64
	SPCR =  (1<<SPE) | (1<<MSTR) | (1<<SPR1) | (0<<SPR0);
	
	// Polarität und Phase für RFM12
	SPCR &= ~(1<<CPHA) & ~(1<<CPOL);
	
	// NOTE: RFM12 verwendet 16bit SPI!
	
	// Einfache SPI-Rate --> fsck/64
	SPSR |= (0 << SPI2X);
	
	// Pull-Ups für SS aktivieren
	SPI_PIN |= (1 << SPI_DD_SS);
	
	// SS hochziehen
	spi_end();
}


/**
 * \brief Begins an SPI session.
 *
 * Starts an SPI session by pulling down {\see SPI_DD_SS}.
 * 
 * \return void
 */
void spi_begin()
{
	SPI_PORT &= ~(1 << SPI_DD_SS);   // SS LOW (enable, low active)
}

/**
 * \brief Ends an SPI session.
 *
 * Ends an SPI session by pulling up {\see SPI_DD_SS}.
 * 
 * \return void
 */
void spi_end()
{ 
	SPI_PORT |= (1 << SPI_DD_SS);   // SS HIGH (disable, low active)
}

/**
 * \brief Transmits a byte through SPI.
 * 
 * \param cData The byte to send.
 * 
 * \return uint_fast8_t Data pushed from the slave.
 */
uint_fast8_t spi_transmit(char cData)
{
	/* Start transmission */
	SPDR = cData;
	
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)))	;
	
	// Gelesene Daten ausgeben
	return SPDR;
}


/**
 * \brief Reads a byte from SPI.
 * 
 * \return uint_fast8_t The read byte
 */
uint_fast8_t spi_read()
{
	return spi_transmit(0);
}
