/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_AFC_H_
#define RFM12_AFC_H_

#include <stdint.h>
#include "IRfm12Command.h"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Automatic Operation Mode
		*/
		typedef enum 
		{
			AUTOMODE_OFF			= 0b00,			//!< Automatic mode off (strobe controlled by µc)
			AUTOMODE_ONCE			= 0b01,			//!< Automatic runs only once after power-up
			AUTOMODE_VDI_HIGH		= 0b10,			//!< Keep f_offset only during data reception (VDI is high)
			AUTOMODE_VDI_INDEPENDENT = 0b11,		//!< Keep f_offset always (independent of VDI)
		} automatic_mode_t;

		/**
		* \brief Range Limits / Maximum Deviation
		*
		* Sets the frequency deviation depending on the frequency resolution f_res (e.g. 433 MHz -> f_res = 2.5 kHz)
		*/
		typedef enum
		{
			MAXDEVIATION_UNRESTRICTED	= 0b00,		//!< No restriction
			MAXDEVIATION_WIDE			= 0b01,		//!< 15 f_res to -16 f_res
			MAXDEVIATION_MEDIUM			= 0b10,		//!< 7 f_res to -8 f_res
			MAXDEVIATION_NARROW			= 0b11,		//!< 3 f_res to -4 f_res
		} range_limit_t;

		/**
		* \brief AFC Command
		*/
		class Rfm12AfcCommand : public IRfm12Command 
		{
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
					* \brief Automatic operatiopn mode selector
					*/
					automatic_mode_t	a:2;
			
					/**
					* \brief Range limit / maximum frequency deviation.
					*/
					range_limit_t		rl:2;
			
					/**
					* \brief Strobed edge.
					*/
					bool				st:1;
			
					/**
					* \brief Use high-accuracy (fine) mode.
					*/
					uint8_t				fi:1;
			
					/**
					* \brief Enable frequency offset register.
					*/
					uint8_t				oe:1;
			
					/**
					* \brief Enable offset frequency calculation.
					*/
					uint8_t				en:1;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			Rfm12AfcCommand() 
				: command_word(0xC4F7)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		};
	}
}

#endif /* RFM12_AFC_H_ */