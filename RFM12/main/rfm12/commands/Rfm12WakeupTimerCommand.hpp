/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_WAKEUPTIMER_H_
#define RFM12_WAKEUPTIMER_H_

#include <stdint.h>
#include "IRfm12Command.h"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Wake-up Timer Command
		*/
		class Rfm12WakeupTimerCommand : public IRfm12Command {
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
					const uint8_t		command_code:3;

					/**
					* \brief Wake-up time period exponential factor.
					*
					* Twake-up = 1.03 · M · 2R + 0.5 [ms]
					*
					* For future compatibility, r should be in range 0..29 inclusive.
					*/
					uint8_t				r:5;
			
					/**
					* \brief Wake-up time period mantissa factor.
					*
					* Twake-up = 1.03 · M · 2R + 0.5 [ms]
					*
					* This parameter also indirectly controls the Low Duty-Cycle Command setting d.
					* \see rfm12_lowdutycycle_command_t
					*/
					uint8_t				m:8;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			Rfm12WakeupTimerCommand() 
				: command_word(0xE196)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Clears the wake-up timer period.
			*
			* Sets the wake-up timer period mantissa and exponent to zero,
			* effectively resulting in a wake-up time of 0.5ms.
			*/
			inline void disableWakeupTimer() { this->r = 0; this->m = 0; }
		};
	}
}

#endif /* RFM12_WAKEUPTIMER_H_ */