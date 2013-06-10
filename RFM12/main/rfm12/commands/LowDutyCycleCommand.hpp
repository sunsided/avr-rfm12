/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_LOWDUTYCYCLE_H_
#define RFM12_LOWDUTYCYCLE_H_

#include <stdint.h>
#include "ICommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Low Duty-Cycle Command
		*/
		class LowDutyCycleCommand : public ICommand {
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief The command code.
					*/
					const uint8_t		command_code:8;		

					/**
					* \brief Low duty-cycle percentage control.
					*
					* This value D controls the length of the low duty-cycle during which
					* no wake-up timer interrupts are generated.
					*
					* duty-cycle = (D � 2 +1) / M � 100%
					*
					* This value is also indirectly controlled by Wake-Up Timer setting M.
					* \see WakeupTimerCommand
					*/
					uint8_t				d:7;
			
					/**
					* \brief Enables the low duty-cycle command
					*/
					bool				en:1;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			LowDutyCycleCommand() 
				: command_word(0xC80E)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the Low Duty-Cycle mode.
			*
			* \param enabled true if the Low Duty-Cycle mode should be enabled, false otherwise.
			*/
			inline void setEnabled(const bool enabled = false) { this->en = enabled; }
		};
	}
}

#endif /* RFM12_LOWDUTYCYCLE_H_ */