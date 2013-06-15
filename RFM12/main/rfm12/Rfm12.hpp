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

#include "commands/CommandResult.hpp"
#include "commands/StatusCommandResult.hpp"

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
		const ISpi* _spi;
	
		/**
		* \brief The receive buffer interface for inbound data
		*/
		const IReceiveBuffer* _receiveBuffer;
		
		/**
		* \brief The send buffer interface for outbound data
		*/
		const ISendBuffer* _sendBuffer;
		
	private:
		/**
		* \brief The command array
		*/
		commands::Command* _commands[RFM12_COMMAND_COUNT];
	
		/**
		* \brief Results of the last status command
		*/
		commands::StatusCommandResult _lastStatus;
		
		/**
		* \brief The last command result
		*/
		commands::CommandResult _lastCommandResult;

	public:
		/**
		* \brief Initializes this instance.
		*
		* \param spi The SPI interface to use.
		*/
		Rfm12(const ISpi* spi, const IReceiveBuffer *receiveBuffer, const ISendBuffer *sendBuffer);
		
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
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result
		 */
		inline const commands::CommandResult* executeCommand(const commands::Command* command)
		{
			return executeCommandRaw(command->getCommandWord());
		}
		
		/**
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result
		 */
		inline const commands::CommandResult* executeCommand(const commands::Command& command)
		{
			return executeCommandRaw(command.getCommandWord());
		}
		
		/**
		 * \brief Gets the last command result as retrieved per executeCommand*()
		 *
		 * \return The result.
		 */
		inline const commands::CommandResult* getCommandResult() const { return &_lastCommandResult; }
		
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
			return static_cast<commands::StatusReadCommand*>(this->_commands[commands::RFM12CMD_STATUS_READ]); 
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
			return static_cast<commands::FifoReadCommand*>(this->_commands[commands::RFM12CMD_RECEIVERFIFO]);
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
			return static_cast<commands::TransmitRegisterWriteCommand*>(this->_commands[commands::RFM12CMD_TRANSMITTERWRITE]);
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
		* \brief Executes an RFM12 command.
		*
		* \param command_code The command to execute.
		*/
		const commands::CommandResult* executeCommandRaw(const uint_least16_t command_code);
	
		/**
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result
		 */
		inline const uint_fast16_t executeCommandInternal(const uint_least16_t command_code) const;
	};

}

#endif /* RFM12_H_ */