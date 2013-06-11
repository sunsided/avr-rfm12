/*
 * rfm12.h
 *
 * Created: 02.06.2013 21:40:57
 *  Author: sunside
 */ 


#ifndef RFM12_H_
#define RFM12_H_

#include <stdint.h>
#include "ISpi.hpp"

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
	 * \brief Initializes the RFM12 interrupt pin.
	 * 
	 * \return void
	 */
	void rfm12_initialize_interrupt();

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
	
	public:
		/**
		* \brief Initializes this instance.
		*
		* \param spi The SPI interface to use.
		*/
		Rfm12(const ISpi* spi) : _spi(spi) {}
	
		/**
		* \brief Executes an RFM12 command.
		*
		* \param command_code The command to execute.
		*/
		const commands::CommandResult executeCommandRaw(const uint_least16_t command_code) const;
	
		/**
		 * \brief Sends a command to the RFM12.
		 *
		 * \param command The command word
		 *
		 * \return The result
		 */
		inline const commands::CommandResult executeCommand(const commands::ICommand* command) const
		{
			return executeCommandRaw(command->getCommandWord());
		}
		
		/**
		 * \brief Reads the status register from the RFM12.
		 *
		 * \return The status.
		 */
		const commands::StatusCommandResult readStatus() const;

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