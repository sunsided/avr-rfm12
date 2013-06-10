/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_POWERMGMT_H_
#define RFM12_POWERMGMT_H_

#include <stdint.h>

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Power Management Command
		*/
		typedef class _rfm12_powermgmt_command_t {
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
					* \brief Enables the whole receiver chain.
					*/
					bool				er:1;

					/**
					* \brief The receiver baseband circuit can be separately switched on
					*/
					bool				ebb:1;
			
					/**
					* \brief Switches on the PLL, the power amplifier, and starts the transmission (If TX register is enabled)
					*/
					bool				et:1;
			
					/**
					* \brief Turns on the synthesizer
					*/
					bool				es:1;
			
					/**
					* \brief Turns on the crystal oscillator
					*/
					bool				ex:1;
			
					/**
					* \brief Enables the low battery detector
					*/
					bool				eb:1;
			
					/**
					* \brief Enables the wake-up timer
					*/
					bool				ew:1;
			
					/**
					* \brief Disables the clock output (pin 8)
					*/
					bool				dc:1;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			_rfm12_powermgmt_command_t() 
				: command_word(0x8208)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

		} Rfm12PowerManagementCommand;

	}
}

#endif /* RFM12_POWERMGMT_H_ */