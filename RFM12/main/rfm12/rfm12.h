/*
 * rfm12.h
 *
 * Created: 02.06.2013 21:40:57
 *  Author: sunside
 */ 


#ifndef RFM12_H_
#define RFM12_H_

#include <stdint.h>
#include "commands/ICommand.h"
#include "commands/CommandResult.h"
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

namespace rfm12
{

	/**
	 * \brief Initializes the RFM12 interrupt pin.
	 * 
	 * \return void
	 */
	void rfm12_initialize_interrupt();

	/**
	 * \brief Sends a command to the RFM12.
	 *
	 * \param command The command word
	 *
	 * \return The result
	 */
	uint_least16_t _Command(const uint_least16_t command);

	/**
	 * \brief Reads the status register from the RFM12.
	 *
	 * \return Status word.
	 */
	uint_least16_t rfm12_read_status();

	/**
	* \brief Implementation of RFM12
	*/
	class Rfm12
	{
	public:
		/**
		* \brief Executes an RFM12 command.
		*
		* \param command The command to execute.
		*/
		commands::CommandResult executeCommand(const commands::ICommand* command) const;
	};

}

#endif /* RFM12_H_ */