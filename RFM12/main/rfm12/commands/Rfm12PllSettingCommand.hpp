/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_PLLSETTING_H_
#define RFM12_PLLSETTING_H_

#include <stdint.h>
#include "../IRfm12Command.h"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Microcontroller output clock buffer rise and fall time.
		*/
		typedef enum {
			MCCLKFRQ_5OR10MHZ		= 0b11,		//<! 5 or 10 MHz (recommended)
			MCCLKFRQ_3POINT3MHZ		= 0b10,		//<! 3.3 MHz
			MCCLKFRQ_2POINT5MHZ		= 0b01,		//<! 2.5 MHz or less
			MCCLKFRQ_LOWEST			= 0b00,		//<! 2.5 MHz or less (same as MCCLKFRQ_2POINT5MHZ)
		} microctrl_clkfrq_t;


		/**
		* \brief PLL Bandwidth Control
		*/
		typedef enum {
			PLLBW_MAX_0862KBPS = 0b0,		//<! Maximum bit rate is  86.2 kbps
			PLLBW_MAX_2560KBPS = 0b1,		//<! Maximum bit rate is 256.0 kbps
		} pll_bandwidth_t;

		/**
		* \brief PLL (Phase-Locked Loop) Setting Command
		*/
		class Rfm12PllSettingCommand : public IRfm12Command {
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
					const uint16_t		command_code:9;

					/**
					* \brief Micro controller output clock buffer rise and fall time control.
					*/
					microctrl_clkfrq_t	ob:2;
			
					/**
					* \brief Padding
					*/
					const uint8_t		:1;
			
					/**
					* \brief Switches on the delay in the phase detector when this bit is set.
					*/
					bool				dly:1;
			
					/**
					* \brief When set, disables the dithering in the PLL loop.
					*/
					bool				ddit:1;
			
					/**
					* \brief Padding
					*/
					uint8_t				:1;
			
					/**
					* \brief PLL bandwidth control for optimal RX/TX performance
					*/
					pll_bandwidth_t		bw:1;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			Rfm12PllSettingCommand() 
				: command_word(0xCC77)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		};
	}
}

#endif /* RFM12_PLLSETTING_H_ */