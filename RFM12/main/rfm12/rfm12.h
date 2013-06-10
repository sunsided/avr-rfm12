/*
 * rfm12.h
 *
 * Created: 02.06.2013 21:40:57
 *  Author: sunside
 */ 


#ifndef RFM12_H_
#define RFM12_H_

#include <stdint.h>
#include "commands/IRfm12Command.h"
#include "commands/Rfm12CommandResult.h"
#include "commands/Rfm12ConfigSetCommand.hpp"
#include "commands/Rfm12PowerManagementCommand.hpp"
#include "commands/Rfm12FrequencyCommand.hpp"
#include "commands/Rfm12DataRateCommand.hpp"
#include "commands/Rfm12ReceiverControlCommand.hpp"
#include "commands/Rfm12DataFilterCommand.hpp"
#include "commands/Rfm12FifoAndResetModeCommand.hpp"
#include "commands/Rfm12SynchronPatternCommand.hpp"
#include "commands/Rfm12FifoReadCommand.hpp"
#include "commands/Rfm12AfcCommand.hpp"
#include "commands/Rfm12TxConfigCommand.hpp"
#include "commands/Rfm12PllSettingCommand.hpp"
#include "commands/Rfm12TransmitRegisterWriteCommand.hpp"
#include "commands/Rfm12WakeupTimerCommand.hpp"
#include "commands/Rfm12LowDutyCycleCommand.hpp"
#include "commands/Rfm12BatteryDetectorAndClockDividerCommand.hpp"
#include "commands/Rfm12StatusReadCommand.hpp"

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
	uint_least16_t rfm12_command(const uint_least16_t command);

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
		commands::Rfm12CommandResult executeCommand(const commands::IRfm12Command* command) const;
	};

}

#endif /* RFM12_H_ */