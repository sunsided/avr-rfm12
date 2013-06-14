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
using namespace commands;

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
const CommandResult& Rfm12::executeCommandRaw(const uint_least16_t command_code)
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
const StatusCommandResult& Rfm12::readStatus()
{
	const uint16_t result = executeCommandInternal(this->_statusCommand);
		
	// wrap in beautiful paper
	_lastStatus.applyResult(result);
	return this->_lastStatus;
}

/**
* \brief Gets the command defined by the id given.
*
* \param id The command id.
*/
ICommand* Rfm12::getCommand(const commandtype_t type) {
	switch (type)
	{
		case RFM12CMD_CONFIGURATION_SETTING:
			return &this->_configSetCommand;
			
		case RFM12CMD_POWERMANAGEMENT:
			return &this->_powerManagementCommand;
			
		case RFM12CMD_FREQUENCYSETTING:
			return &this->_frequencyCommand;
			
		case RFM12CMD_DATARATE:
			return &this->_dataRateCommand;
			
		case RFM12CMD_RECEIVERCONTROL:
			return &this->_receiverControlCommand;
			
		case RFM12CMD_DATAFILTER:
			return &this->_dataFilterCommand;
			
		case RFM12CMD_FIFOANDRESETMODE:
			return &this->_fifoAndResetModeCommand;
			
		case RFM12CMD_SYNCHRONPATTERN:
			return &this->_synchronPatternCommand;
			
		case RFM12CMD_RECEIVERFIFO:
			return &this->_fifoReadCommand;
			
		case RFM12CMD_AFC:
			return &this->_afcCommand;
			
		case RFM12CMD_TXCONFIGURATION:
			return &this->_txConfigCommand;
			
		case RFM12CMD_PLLSETTING:
			return &this->_pllSettingCommand;
			
		case RFM12CMD_TRANSMITTERWRITE:
			return &this->_txWriteCommand;
		
		case RFM12CMD_WAKEUPTIMER:
			return &this->_wakeUpTimerCommand;
		
		case RFM12CMD_LOWDUTYCYCLE:
			return &this->_lowDutyCycleCommand;
		
		case RFM12CMD_LOWBATTERY_MCCLOCKDIVDER:
			return &this->_batteryAndMicroCommand;
		
		case RFM12CMD_STATUS_READ:
			return &this->_statusCommand;
	}
	return NULL;
}

/**
* \brief Drives the internal communication system.
*
* This method should be called on every interrupt event from the RFM12's nIRQ line.
*/	
void Rfm12::pulse() 
{
	
}