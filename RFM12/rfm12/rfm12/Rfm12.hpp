/*
 * rfm12.h
 *
 * Created: 02.06.2013 21:40:57
 *  Author: sunside
 */ 


#ifndef RFM12_H_
#define RFM12_H_

#include <stdint.h>
#include <assert.h>
#include "ISpi.hpp"
#include "IReceiveBuffer.hpp"
#include "ISendBuffer.hpp"

#include "commands/Command.hpp"
#include "commands/ConfigSetCommand.hpp"
#include "commands/PowerManagementCommand.hpp"
#include "commands/FrequencyCommand.hpp"
#include "commands/DataRateCommand.hpp"
#include "commands/ReceiverControlCommand.hpp"
#include "commands/DataFilterCommand.hpp"
#include "commands/FifoAndResetModeCommand.hpp"
#include "commands/SynchronPatternCommand.hpp"
#include "commands/FifoReadCommand.hpp"
#include "commands/AfcCommand.hpp"
#include "commands/TxConfigCommand.hpp"
#include "commands/PllSettingCommand.hpp"
#include "commands/TransmitRegisterWriteCommand.hpp"
#include "commands/WakeupTimerCommand.hpp"
#include "commands/LowDutyCycleCommand.hpp"
#include "commands/BatteryDetectorAndClockDividerCommand.hpp"
#include "commands/StatusReadCommand.hpp"

#include "results/CommandResult.hpp"
#include "results/StatusCommandResult.hpp"

#include "TransceiverStrategy.hpp"
#include "TransceiverMode.hpp"

namespace rfm12
{

	/**
	* \brief Implementation of RFM12
	*/
	class Rfm12
	{
	private:
		/**
		* \brief The SPI interface used for communication.
		*/
		ISpi *const _spi;
	
		/**
		* \brief The receive buffer interface for inbound data
		*/
		IReceiveBuffer *const _receiveBuffer;
		
		/**
		* \brief The send buffer interface for outbound data
		*/
		ISendBuffer *const _sendBuffer;
		
		/**
		* \brief The command array
		*/
		//commands::Command *_commands[RFM12_COMMAND_COUNT];
		commands::Command **const _commands;
	
		/**
		* \brief Results of the last status command
		*/
		commands::StatusCommandResult _lastStatus;
		
		/**
		* \brief The last command result
		*/
		commands::CommandResult _lastCommandResult;

		/**
		* \brief The transceiver strategy
		*/
		transceiverstrategy_t _transceiverStrategy;
		
		/**
		* \brief The transceiver mode
		*/
		transceivermode_t _transceiverMode;

		/**
		* \brief Pointer for fast access to the write command
		*/
		//commands::TransmitRegisterWriteCommand *const _txWriteFastAccess;
		commands::TransmitRegisterWriteCommand *_txWriteFastAccess;
		
		/**
		* \brief Pointer for fast access to the read command
		*/
		//commands::FifoReadCommand *const _receiverReadFastAccess;
		commands::FifoReadCommand *_receiverReadFastAccess;

		/**
		* \brief Pointer for fast access to the status read command
		*/
		//commands::StatusReadCommand *const _statusReadFastAccess;
		commands::StatusReadCommand *_statusReadFastAccess;
		
		/**
		* \brief Determines if the current transmission is done.
		*
		* This value will be set automatically depending on the number of bytes in
		* the send buffer. If no byte could be read, transmission is considered done.
		*/
		volatile bool _transmissionDone;

	public:
		/**
		* \brief Initializes this instance.
		*
		* \param spi The SPI interface to use.
		*/
		Rfm12(ISpi* spi, IReceiveBuffer *receiveBuffer, ISendBuffer *sendBuffer);
		
		/**
		* \brief Destructor
		*/
		~Rfm12();
		
		/**
		* \brief Drives the internal communication system.
		*
		* This method should be called on every interrupt event from the RFM12's nIRQ line.
		*/	
		void pulse();
				
		/**
		 * \brief Reads the status register from the RFM12.
		 *
		 * \return The status.
		 */
		const commands::StatusCommandResult* readStatus();

		/**
		 * \brief Gets the last status as retrieved per readStatus()
		 *
		 * \return The status.
		 */
		inline const commands::StatusCommandResult* getLastStatus() const { return &_lastStatus; }

		/**
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result or NULL if the command was invalid
		 */
		const commands::CommandResult* executeCommand(const commands::Command* command);
				
		/**
		 * \brief Gets the last command result as retrieved per executeCommand*()
		 *
		 * \return The result.
		 */
		inline const commands::CommandResult* getCommandResult() const { return &_lastCommandResult; }

		/**
		* \brief Gets the transceiver mode.
		*
		* \returns The transceiver mode.
		*/
		inline const transceivermode_t getMode() const { return _transceiverMode; }

		/**
		* \brief Adjusts the Power Management Command for the given transceiver strategy.
		*
		* This might implicitly issue an SPI transmission.
		*
		* \param command The command
		* \param mode The selected mode
		* \param forceCommit If set to true, an SPI transfer will be initiated regardless of the current transceiver mode.
		*/
		void setTransceiverMode(register const transceivermode_t mode = RXTXMODE_RX, register const bool forceCommit = false);

		/**
		* \brief Enters transmit (TX) mode
		*
		* This might implicitly issue an SPI transmission.
		*
		* \param forceCommit If set to true, an SPI transfer will be initiated regardless of the current transceiver mode.
		*/
		inline void enterTransmitterMode(const bool forceCommit = false) { setTransceiverMode(RXTXMODE_TX, forceCommit); }
		
		/**
		* \brief Enters receive (RX) mode
		*
		* This might implicitly issue an SPI transmission.
		*
		* \param forceCommit If set to true, an SPI transfer will be initiated regardless of the current transceiver mode.
		*/
		void enterReceiverMode(const bool forceCommit = false) { setTransceiverMode(RXTXMODE_RX, forceCommit); }
		
		/**
		* \brief Restarts receiver synchronization.
		*
		* Forces an re-sync by toggling or enabling the enable fifo bit.
		*/
		inline void resyncReceiver() {
			commands::ConfigSetCommand *config = getConfigSetCommand();
			
			// if the FIFO is still enabled, disable it first
			if (config->getFifoEnabled()) {
				config->setFifoEnabled(false);
				executeCommand(config);
			}
			
			// enable FIFO to start pattern matching
			config->setFifoEnabled(true);
			executeCommand(config);
		}
		
		/**
		* \brief Enters idle mode (i.e. leaves transmitter and receiver mode)
		*
		* This might implicitly issue an SPI transmission.
		*
		* \param forceCommit If set to true, an SPI transfer will be initiated regardless of the current transceiver mode.
		*/
		void enterIdleMode(const bool forceCommit = false) { setTransceiverMode(RXTXMODE_IDLE, forceCommit); }
			
		/**
		* \brief Sets the transceiver strategy.
		*
		* This might implicitly issue an SPI transmission.
		*
		* \param strategy The selected mode
		* \param forceCommit If set to true, an SPI transfer will be initiated regardless of the current transceiver mode.
		*/
		inline void setTransceiverStrategy(register const transceiverstrategy_t strategy = RXTXSTRATEGY_FAST_SWITCHING, const bool forceCommit = false) {
			_transceiverStrategy = strategy;
			if (forceCommit) setTransceiverMode(_transceiverMode, true);
		}

		/**
		* \brief Determines whether the current transmission is considered done.
		*
		* \returns The transmission state
		*/
		inline const bool isTransmissionDone() const { return _transmissionDone; }

	public:
	
		/**
		* \brief Gets the command defined by the id given.
		*
		* \param id The command id.
		* \return The command
		*/
		inline commands::Command* getCommand(const commands::commandtype_t type) {
			return _commands[type];
		}
	
		/**
		* \brief The status command
		*/
		const inline commands::StatusReadCommand* getStatusCommand() const { 
			return _statusReadFastAccess;
			// return static_cast<commands::StatusReadCommand*>(this->_commands[commands::RFM12CMD_STATUS_READ]); 
		}
			
		/**
		* \brief Configuration Setting Command
		*/
		inline commands::ConfigSetCommand* getConfigSetCommand() {
			return static_cast<commands::ConfigSetCommand*>(this->_commands[commands::RFM12CMD_CONFIGURATION_SETTING]);
		}
		
		/**
		* \brief Power Management Command
		*/
		inline commands::PowerManagementCommand* getPowerManagementCommand() {
			return static_cast<commands::PowerManagementCommand*>(this->_commands[commands::RFM12CMD_POWERMANAGEMENT]);
		}
		
		/**
		* \brief Frequency Setting Command
		*/
		inline commands::FrequencyCommand* getFrequencyCommand() {
			return static_cast<commands::FrequencyCommand*>(this->_commands[commands::RFM12CMD_FREQUENCYSETTING]);
		}
		
		/**
		* \brief Data Rate Command
		*/
		inline commands::DataRateCommand* getDataRateCommand() {
			return static_cast<commands::DataRateCommand*>(this->_commands[commands::RFM12CMD_DATARATE]);
		}
		
		/**
		* \brief Receiver Control Command
		*/
		inline commands::ReceiverControlCommand* getReceiverControlCommand() {
			return static_cast<commands::ReceiverControlCommand*>(this->_commands[commands::RFM12CMD_RECEIVERCONTROL]);
		}
		
		/**
		* \brief Data Filter Command
		*/
		inline commands::DataFilterCommand* getDataFilterCommand() {
			return static_cast<commands::DataFilterCommand*>(this->_commands[commands::RFM12CMD_DATAFILTER]);
		}
		
		/**
		* \brief FIFO and Reset Mode Command
		*/
		inline commands::FifoAndResetModeCommand* getFifoAndResetModeCommand() {
			return static_cast<commands::FifoAndResetModeCommand*>(this->_commands[commands::RFM12CMD_FIFOANDRESETMODE]);
		}
		
		/**
		* \brief Synchron Pattern Command
		*/
		inline commands::SynchronPatternCommand* getSynchronPatternCommand() {
			return static_cast<commands::SynchronPatternCommand*>(this->_commands[commands::RFM12CMD_SYNCHRONPATTERN]);
		}
		
		/**
		* \brief Receiver FIFO Read Command
		*/
		const inline commands::FifoReadCommand* getFifoReadCommand() const {
			return _receiverReadFastAccess;
			// return static_cast<commands::FifoReadCommand*>(this->_commands[commands::RFM12CMD_RECEIVERFIFO]);
		}
		
		/**
		* \brief AFC Command
		*/
		inline commands::AfcCommand* getAfcCommand() {
			return static_cast<commands::AfcCommand*>(this->_commands[commands::RFM12CMD_AFC]);
		}
		
		/**
		* \brief TX Configuration Control Command
		*/
		inline commands::TxConfigCommand* getTxConfigCommand() {
			return static_cast<commands::TxConfigCommand*>(this->_commands[commands::RFM12CMD_TXCONFIGURATION]);
		}
		
		/**
		* \brief PLL Setting Command
		*/
		inline commands::PllSettingCommand* getPllSettingCommand() {
			return static_cast<commands::PllSettingCommand*>(this->_commands[commands::RFM12CMD_PLLSETTING]);
		}
		
		/**
		* \brief Transmitter Register Write Command
		*/
		inline commands::TransmitRegisterWriteCommand* getTransmitRegisterWrite() {
			return _txWriteFastAccess;
			// return static_cast<commands::TransmitRegisterWriteCommand*>(this->_commands[commands::RFM12CMD_TRANSMITTERWRITE]);
		}
		
		/**
		* \brief Wake-Up Timer Command
		*/
		inline commands::WakeupTimerCommand* getWakeUpTimerCommand() {
			return static_cast<commands::WakeupTimerCommand*>(this->_commands[commands::RFM12CMD_WAKEUPTIMER]);
		}
		
		/**
		* \brief Low Duty-Cycle Command
		*/
		inline commands::LowDutyCycleCommand* getLowDutyCycleCommand() {
			return static_cast<commands::LowDutyCycleCommand*>(this->_commands[commands::RFM12CMD_LOWDUTYCYCLE]);
		}
		
		/**
		* \brief Low Battery Detector and Microcontroller Clock Divider Command
		*/
		inline commands::BatteryDetectorAndClockDividerCommand* getBatteryAndMicroCommand() {
			return static_cast<commands::BatteryDetectorAndClockDividerCommand*>(this->_commands[commands::RFM12CMD_LOWBATTERY_MCCLOCKDIVDER]);
		}

	private:
		/**
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result
		 */
		inline const uint_fast16_t executeCommandInternalRaw(const uint_least16_t command_code) const;
		
		/**
		* \brief Drives the internal communication system; Called internally when in transmitter mode.
		*/	
		void pulseTx(register const commands::StatusCommandResult *status);
		
		/**
		* \brief Drives the internal communication system; Called internally when in receiver mode.
		*/	
		void pulseRx(register const commands::StatusCommandResult *status);
	};

}

#endif /* RFM12_H_ */