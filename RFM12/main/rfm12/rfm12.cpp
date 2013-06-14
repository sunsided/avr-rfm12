/*
 * rfm12.c
 *
 * Created: 02.06.2013 21:40:16
 *  Author: sunside
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "../main.h"
#include "ISpi.hpp"
#include "Rfm12.hpp"

using namespace rfm12;

/**
	* \brief Sends a command to the RFM12.
	*
	* \param command The command word
	*
	* \return The result
	*/
inline const uint_fast16_t Rfm12::executeCommandInternal(const uint_least16_t command_code) const
{
	_spi->beginTransmission();
	uint_fast16_t result = _spi->transmitWord(command_code);
	_spi->endTransmission();

	// uint_fast16_t result  = spi_transmit((command_code >> 8) & 0xFF) << 8;
	// 			   result |= spi_transmit(command_code & 0xFF);
		
	return result;
}

/**
	* \brief Sends a command to the RFM12.
	*
	* \param command The command word
	*
	* \return The result
	*/
const commands::CommandResult& Rfm12::executeCommandRaw(const uint_least16_t command_code)
{
	const uint_fast16_t result = executeCommandInternal(command_code);
		
	// wrap in beautiful paper
	_lastCommandResult.applyResult(result);
	return _lastCommandResult;
}

/**
	* \brief Reads the status register from the RFM12.
	*
	* \return Status byte
	*/
const commands::StatusCommandResult& Rfm12::readStatus()
{
	const uint16_t result = executeCommandInternal(this->_statusCommand);
		
	// wrap in beautiful paper
	_lastStatus.applyResult(result);
	return this->_lastStatus;
}

/**
* \brief Drives the internal communication system.
*
* This method should be called on every interrupt event from the RFM12's nIRQ line.
*/	
void Rfm12::pulse() 
{
	
}