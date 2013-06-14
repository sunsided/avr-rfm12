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

#include "commands/ICommand.hpp"
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
		* \brief The status command
		*/
		commands::StatusReadCommand _statusCommand;
	
		/**
		* \brief Results of the last status command
		*/
		commands::StatusCommandResult _lastStatus;
		
		/**
		* \brief The last command result
		*/
		commands::CommandResult _lastCommandResult;

		/**
		* \brief Configuration Setting Command
		*/
		commands::ConfigSetCommand _configSetCommand;
		
		/**
		* \brief Power Management Command
		*/
		commands::PowerManagementCommand _powerManagementCommand;
		
		/**
		* \brief Frequency Setting Command
		*/
		commands::FrequencyCommand _frequencyCommand;
		
		/**
		* \brief Data Rate Command
		*/
		commands::DataRateCommand _dataRateCommand;	
		
		/**
		* \brief Receiver Control Command
		*/
		commands::ReceiverControlCommand _receiverControlCommand;	
		
		/**
		* \brief Data Filter Command
		*/
		commands::DataFilterCommand _dataFilterCommand;	
		
		/**
		* \brief FIFO and Reset Mode Command
		*/
		commands::FifoAndResetModeCommand _fifoAndResetModeCommand;	
		
		/**
		* \brief Synchron Pattern Command
		*/
		commands::SynchronPatternCommand _synchronPatternCommand;	
		
		/**
		* \brief Receiver FIFO Read Command
		*/
		commands::FifoReadCommand _fifoReadCommand;	
		
		/**
		* \brief AFC Command
		*/
		commands::AfcCommand _afcCommand;	
		
		/**
		* \brief TX Configuration Control Command
		*/
		commands::TxConfigCommand _txConfigCommand;	
		
		/**
		* \brief PLL Setting Command
		*/
		commands::PllSettingCommand _pllSettingCommand;	
		
		/**
		* \brief Transmitter Register Write Command
		*/
		commands::TransmitRegisterWriteCommand _txWriteCommand;	
		
		/**
		* \brief Wake-Up Timer Command
		*/
		commands::WakeupTimerCommand _wakeUpTimerCommand;	
		
		/**
		* \brief Low Duty-Cycle Command
		*/
		commands::LowDutyCycleCommand _lowDutyCycleCommand;	
		
		/**
		* \brief Low Battery Detector and Microcontroller Clock Divider Command
		*/
		commands::BatteryDetectorAndClockDividerCommand _batteryAndMicroCommand;
		
	public:
		/**
		* \brief Initializes this instance.
		*
		* \param spi The SPI interface to use.
		*/
		inline Rfm12(const ISpi* spi, const IReceiveBuffer *receiveBuffer, const ISendBuffer *sendBuffer) 
		: _spi(spi), _receiveBuffer(receiveBuffer), _sendBuffer(sendBuffer)
		{
			assert(NULL != spi);
			assert(NULL != receiveBuffer);
			assert(NULL != sendBuffer);
		}
		
		/**
		* \brief Drives the internal communication system.
		*
		* This method should be called on every interrupt event from the RFM12's nIRQ line.
		*/	
		void pulse();
	
		/**
		* \brief Executes an RFM12 command.
		*
		* \param command_code The command to execute.
		*/
		const commands::CommandResult& executeCommandRaw(const uint_least16_t command_code);
	
		/**
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result
		 */
		inline const commands::CommandResult& executeCommand(const commands::ICommand* command)
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
		inline const commands::CommandResult& executeCommand(const commands::ICommand& command)
		{
			return executeCommandRaw(command.getCommandWord());
		}
		
		/**
		 * \brief Gets the last command result as retrieved per executeCommand*()
		 *
		 * \return The result.
		 */
		inline const commands::CommandResult& getCommandResult() const { return this->_lastCommandResult; }
		
		/**
		 * \brief Reads the status register from the RFM12.
		 *
		 * \return The status.
		 */
		const commands::StatusCommandResult& readStatus();

		/**
		 * \brief Gets the last status as retrieved per readStatus()
		 *
		 * \return The status.
		 */
		inline const commands::StatusCommandResult& getLastStatus() const { return this->_lastStatus; }

		/**
		* \brief Gets the command defined by the id given.
		*
		* \param id The command id.
		*/
		commands::ICommand* getCommand(const commands::commandtype_t type);
		
	public:
	
		/**
		* \brief The status command
		*/
		const inline commands::StatusReadCommand* getStatusCommand() const { return &this->_statusCommand; }
			
		/**
		* \brief Configuration Setting Command
		*/
		inline commands::ConfigSetCommand* getConfigSetCommand() { return &this->_configSetCommand; }
		
		/**
		* \brief Power Management Command
		*/
		inline commands::PowerManagementCommand* getPowerManagementCommand() { return &this->_powerManagementCommand; }
		
		/**
		* \brief Frequency Setting Command
		*/
		inline commands::FrequencyCommand* getFrequencyCommand() { return &this->_frequencyCommand; }
		
		/**
		* \brief Data Rate Command
		*/
		inline commands::DataRateCommand* getDataRateCommand() { return &this->_dataRateCommand; }
		
		/**
		* \brief Receiver Control Command
		*/
		inline commands::ReceiverControlCommand* getReceiverControlCommand() { return &this->_receiverControlCommand; }
		
		/**
		* \brief Data Filter Command
		*/
		inline commands::DataFilterCommand* getDataFilterCommand() { return &this->_dataFilterCommand; }
		
		/**
		* \brief FIFO and Reset Mode Command
		*/
		inline commands::FifoAndResetModeCommand* getFifoAndResetModeCommand() { return &this->_fifoAndResetModeCommand; }
		
		/**
		* \brief Synchron Pattern Command
		*/
		inline commands::SynchronPatternCommand* getSynchronPatternCommand() { return &this->_synchronPatternCommand; }
		
		/**
		* \brief Receiver FIFO Read Command
		*/
		inline commands::FifoReadCommand* getFifoReadCommand() { return &this->_fifoReadCommand; }
		
		/**
		* \brief AFC Command
		*/
		inline commands::AfcCommand* getAfcCommand() { return &this->_afcCommand; }
		
		/**
		* \brief TX Configuration Control Command
		*/
		inline commands::TxConfigCommand* getTxConfigCommand() { return &this->_txConfigCommand; }
		
		/**
		* \brief PLL Setting Command
		*/
		inline commands::PllSettingCommand* getPllSettingCommand() { return &this->_pllSettingCommand; }
		
		/**
		* \brief Transmitter Register Write Command
		*/
		inline commands::TransmitRegisterWriteCommand* TransmitRegisterWrite() { return &this->_txWriteCommand; }
		
		/**
		* \brief Wake-Up Timer Command
		*/
		inline commands::WakeupTimerCommand* getWakeUpTimerCommand() { return &this->_wakeUpTimerCommand; }
		
		/**
		* \brief Low Duty-Cycle Command
		*/
		inline commands::LowDutyCycleCommand* getLowDutyCycleCommand() { return &this->_lowDutyCycleCommand; }
		
		/**
		* \brief Low Battery Detector and Microcontroller Clock Divider Command
		*/
		inline commands::BatteryDetectorAndClockDividerCommand* getBatteryAndMicroCommand() { return &this->_batteryAndMicroCommand; }

	private:
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