/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_LOWDUTYCYCLE_H_
#define RFM12_LOWDUTYCYCLE_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Low Duty-Cycle Command
		*/
		class LowDutyCycleCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {	
					/**
					* \brief Enables the low duty-cycle command
					*/
					bool				en:1;
					
					/**
					* \brief Low duty-cycle percentage control.
					*
					* This value D controls the length of the low duty-cycle during which
					* no wake-up timer interrupts are generated.
					*
					* duty-cycle = (D · 2 +1) / M · 100%
					*
					* This value is also indirectly controlled by Wake-Up Timer setting M.
					* \see WakeupTimerCommand
					*/
					uint8_t				d:7;
					
					/**
					* \brief The command code.
					*/
					const uint8_t		command_code:8;		
				};
			};
	
			private:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			LowDutyCycleCommand(Rfm12 *owner) 
				: Command(owner), command_word(0xC80E)
			{}
				
			public:

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the duty-cycle.
			*
			* The duty-cycle can be calculated by using (d6 to d0) and M. (M is parameter in a Wake-Up Timer Command, see
			* above). The time cycle is determined by the Wake-Up Timer Command.
			* duty-cycle= (D · 2 +1) / M · 100%
			*
			* \param value The duty cycle factor.
			*/
			inline void setDutyCycle(const uint8_t value = 7) { this->d = value; }

			/**
			* \brief Sets the Low Duty-Cycle mode.
			*
			* \param enabled true if the Low Duty-Cycle mode should be enabled, false otherwise.
			*/
			inline void setEnabled(const bool enabled = false) { this->en = enabled; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_LOWDUTYCYCLE;
			}
		};
	}
}

#endif /* RFM12_LOWDUTYCYCLE_H_ */