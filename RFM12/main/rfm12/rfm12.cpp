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

Rfm12::Rfm12(const ISpi* spi, const IReceiveBuffer *receiveBuffer, const ISendBuffer *sendBuffer)
:	_spi(spi), _receiveBuffer(receiveBuffer), _sendBuffer(sendBuffer), 
	_transceiverStrategy(RXTXSTRATEGY_IGNORE),
	_transceiverMode(RXTXMODE_IDLE)
{
	assert(NULL != spi);
	assert(NULL != receiveBuffer);
	assert(NULL != sendBuffer);
	
	// TODO: Extrahieren und komplettes Konfigurationsverhalten under the hood lösen
	
	_commands[RFM12CMD_CONFIGURATION_SETTING]	= new ConfigSetCommand();
	_commands[RFM12CMD_POWERMANAGEMENT]			= new PowerManagementCommand();
	_commands[RFM12CMD_FREQUENCYSETTING]		= new FrequencyCommand();
	_commands[RFM12CMD_DATARATE]				= new DataRateCommand();
	_commands[RFM12CMD_RECEIVERCONTROL]			= new ReceiverControlCommand();
	_commands[RFM12CMD_DATAFILTER]				= new DataFilterCommand();
	_commands[RFM12CMD_FIFOANDRESETMODE]		= new FifoAndResetModeCommand();
	_commands[RFM12CMD_SYNCHRONPATTERN]			= new SynchronPatternCommand();
	_commands[RFM12CMD_RECEIVERFIFO]			= new FifoReadCommand();
	_commands[RFM12CMD_AFC]						= new AfcCommand();
	_commands[RFM12CMD_TXCONFIGURATION]			= new TxConfigCommand();
	_commands[RFM12CMD_PLLSETTING]				= new PllSettingCommand();
	_commands[RFM12CMD_TRANSMITTERWRITE]		= new TransmitRegisterWriteCommand();
	_commands[RFM12CMD_WAKEUPTIMER]				= new WakeupTimerCommand();
	_commands[RFM12CMD_LOWDUTYCYCLE]			= new LowDutyCycleCommand();
	_commands[RFM12CMD_LOWBATTERY_MCCLOCKDIVDER] = new BatteryDetectorAndClockDividerCommand();
	_commands[RFM12CMD_STATUS_READ]				= new StatusReadCommand();
	
	#ifdef DEBUG
	for (uint8_t i=0; i<RFM12_COMMAND_COUNT; ++i) {
		assert(_commands[i] != NULL);
	}
	#endif
}

/**
* \brief Destructor
*/
Rfm12::~Rfm12() {
	for (uint8_t i=0; i<RFM12_COMMAND_COUNT; ++i) {
		delete _commands[i];
		_commands[i] = NULL;
	}		
}

/**
	* \brief Sends a command to the RFM12.
	*
	* \param command The command word
	*
	* \return The result
	*/
inline const uint_fast16_t Rfm12::executeCommandInternalRaw(const uint_least16_t command_code) const
{
	_spi->beginTransmission();
	uint_fast16_t result = _spi->transmitWord(command_code);
	_spi->endTransmission();

	return result;
}


/**
* \brief Adjusts the Power Management Command for the given transceiver strategy.
*
* \param command The command
* \param strategy The selected strategy.
*/
static inline void adjustForTransceiverStrategy(register PowerManagementCommand *command, register const transceiverstrategy_t strategy)
{
	switch (strategy)
	{
		default:
		case RXTXSTRATEGY_IGNORE:
		{
			break;	
		}
		
		case RXTXSTRATEGY_FAST_SWITCHING: 
		{
			command->setSynthesizerEnabled(true);
			command->setCrystalOscillatorEnabled(true);
			command->setReceiverBasebandCircuitryEnabled(true);
			break;
		}
	}
}

/**
* \brief Sends a command to the RFM12.
*
* \param command The command word
*
* \return The result or NULL if the command was invalid
*/
const CommandResult* Rfm12::executeCommand(const Command* command)
{
	const uint16_t command_code = command->getCommandWord();
	
	// apply the command word to the internal values if they match
	const commandtype_t type = command->getCommandType();
	Command* token = _commands[type];
	bool match = (command == token || token->applyCommandWord(command_code));
	
	// no matching command was found, abort.
	if (!match) return NULL;
	
	// if it is a power management command, fetch the current transceiver mode
	if (type == RFM12CMD_POWERMANAGEMENT) {
		PowerManagementCommand *mgmt = static_cast<PowerManagementCommand*>(token);
		_transceiverMode = mgmt->er ? RXTXMODE_RX : (mgmt->et ? RXTXMODE_TX : RXTXMODE_IDLE);
		
		// apply transceiver strategy here.
		// if the strategy is set to RXTXSTRATEGY_IGNORE,
		// the user settings will not be overwritten.
		adjustForTransceiverStrategy(mgmt, _transceiverStrategy);
	}
	
	// execute
	const uint_fast16_t result = executeCommandInternalRaw(command_code);
	
	// wrap in beautiful paper
	_lastCommandResult.applyResult(result);
	return &_lastCommandResult;
}

/**
* \brief Reads the status register from the RFM12.
*
* \return Status byte
*/
const StatusCommandResult* Rfm12::readStatus()
{
	const uint16_t result = executeCommandInternalRaw(*this->_commands[RFM12CMD_STATUS_READ]);
		
	// wrap in beautiful paper
	_lastStatus.applyResult(result);
	return &_lastStatus;
}

/**
* \brief Adjusts the Power Management Command for the given transceiver strategy.
*
* \param command The command
* \param mode The selected mode
*/
inline void Rfm12::setTransceiverMode(register const transceivermode_t mode) 
{
	if (mode == _transceiverMode) return;
	
	PowerManagementCommand *command = getPowerManagementCommand();
	command->setReceiverBasebandCircuitryEnabled(RXTXMODE_RX == mode);
	command->setTransmissionEnabled(RXTXMODE_TX == mode);

	// NOTE: strategy adjustment is done in the command execution phase.
	executeCommand(command);
}

/**
* \brief Drives the internal communication system.
*
* This method should be called on every interrupt event from the RFM12's nIRQ line.
*/	
void Rfm12::pulse() 
{
	
}