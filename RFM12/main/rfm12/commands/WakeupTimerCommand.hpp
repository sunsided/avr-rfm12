/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_WAKEUPTIMER_H_
#define RFM12_WAKEUPTIMER_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Wake-up Timer Command
		*/
		class WakeupTimerCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief Wake-up time period mantissa factor.
					*
					* Twake-up = 1.03 · M · 2R + 0.5 [ms]
					*
					* This parameter also indirectly controls the Low Duty-Cycle Command setting d.
					* \see LowDutyCycleCommand
					*/
					uint8_t				m:8;
					
					/**
					* \brief Wake-up time period exponential factor.
					*
					* Twake-up = 1.03 · M · 2R + 0.5 [ms]
					*
					* For future compatibility, r should be in range 0..29 inclusive.
					*/
					uint8_t				r:5;
					
					/**
					* \brief The command code.
					*/
					const uint8_t		command_code:3;
				};
			};
	
			private:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			WakeupTimerCommand(Rfm12 *owner) 
				: Command(owner), command_word(0xE196)
			{}
				
			public:				

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the factors for the wake-up timer.
			*
			* The wake-up time period can be calculated by:
			* Twake-up = 1.03 · M · 2R + 0.5 [ms]
			*
			* \param mantissa The mantissa factor
			* \param exponent The exponential factor
			*/
			inline void setWakeupTimerFactors(const uint8_t mantissa = 0x96, const uint8_t exponent = 0x1)
			{
				this->m = mantissa;
				this->r = exponent < 0b11111 ? exponent : 0b11111;
			}

			/**
			* \brief Clears the wake-up timer period.
			*
			* Sets the wake-up timer period mantissa and exponent to zero,
			* effectively resulting in a wake-up time of 0.5ms.
			*/
			inline void disableWakeupTimer() { this->r = 0; this->m = 0; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_WAKEUPTIMER;
			}
		};
	}
}

#endif /* RFM12_WAKEUPTIMER_H_ */