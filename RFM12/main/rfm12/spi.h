/*
 * spi.h
 *
 * Created: 02.06.2013 21:22:00
 *  Author: sunside
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

#define SPI_DDR			DDRB
#define SPI_PORT		PORTB
#define SPI_PIN			PINB

#define SPI_DD_MOSI		PORTB3
#define SPI_DD_MISO		PORTB4
#define SPI_DD_SCK		PORTB5
#define SPI_DD_SS		PORTB2

#define SPI_READ_ADDRESS(address)   ((address) | 0x80U)
#define SPI_WRITE_ADDRESS(address)  ((address) & 0x7FU)

/**
 * \brief Initializes the SPI interface.
 * 
 * \return void
 */
void spi_initialize();

/**
 * \brief Begins an SPI session.
 *
 * Starts an SPI session by pulling down {\see SPI_DD_SS}.
 * 
 * \return void
 */
void spi_begin();

/**
 * \brief Ends an SPI session.
 *
 * Ends an SPI session by pulling up {\see SPI_DD_SS}.
 * 
 * \return void
 */
void spi_end();

/**
 * \brief Transmits a byte through SPI.
 * 
 * \param cData The byte to send.
 * 
 * \return uint_fast8_t Data pushed from the slave.
 */
uint_fast8_t spi_transmit(char cData);

/**
 * \brief Reads a byte from SPI.
 * 
 * \return uint_fast8_t The read byte
 */
uint_fast8_t spi_read();



#endif /* SPI_H_ */