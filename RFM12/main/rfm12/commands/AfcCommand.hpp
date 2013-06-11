/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_AFC_H_
#define RFM12_AFC_H_

#include <stdint.h>
#include "ICommand.hpp"

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
		class AfcCommand : public ICommand 
		{
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief Enable offset frequency calculation.
					*/
					bool				en:1;
					
					/**
					* \brief Enable frequency offset register.
					*/
					bool				oe:1;
					
					/**
					* \brief Use high-accuracy (fine) mode.
					*/
					bool				fi:1;
					
					/**
					* \brief Strobed edge.
					*/
					bool				st:1;
					
					/**
					* \brief Range limit / maximum frequency deviation.
					*/
					range_limit_t		rl:2;
					
					/**
					* \brief Automatic operatiopn mode selector
					*/
					automatic_mode_t	a:2;
					
					/**
					* \brief The command code.
					*/
					const uint8_t		command_code:8;		
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			AfcCommand() 
				: command_word(0xC4F7)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
				
			/**
			* \brief Sets the automatic operation mode.
			*
			* \param value The automatic operation mode.
			*/
			inline void setAutomaticOperationMode(const automatic_mode_t value = AUTOMODE_VDI_INDEPENDENT) { this->a = value; }
				
			/**
			* \brief Sets the range limits.
			*
			* \param value The value, the frequency offset register is limited to.
			*/
			inline void setRangeLimit(const range_limit_t value = MAXDEVIATION_NARROW) { this->rl = value; }
				
			/**
			* \brief Enables strobe edge mode.
			*
			* Strobe edge, when st goes to high, the actual latest calculated frequency error is stored into the offset register of
			* the AFC block.
			*
			* \param value Strobe edge enabled if true, strobe edge disabled otherwise.
			*/
			inline void setStrobeEdgeEnabled(const bool enabled = true) { this->st = enabled; }
				
			/**
			* \brief Switches the circuit to high accuracy (fine) mode.
			*
			* In fine mode, the processing time is about twice as long, but the measurement uncertainty is about half.
			*
			* \param value true if fine mode is enabled, otherwise false.
			*/
			inline void setFineModeEnabled(const bool enabled = true) { this->fi = enabled; }
				
			/**
			* \brief Enables the frequency offset register.
			*
			* The offset register allows the addition of the offset register to the frequency control word of the PLL.
			*
			* \param value Offset register enabled if true, disabled otherwise.
			*/
			inline void setFrequencyOffsetRegisterEnabled(const bool enabled = true) { this->oe = enabled; }
				
			/**
			* \brief Enables the calculation of the offset frequency by the AFC circuit.
			*
			* \param value Offset calculation enabled if true, disabled otherwise.
			*/
			inline void setFrequencyOffsetCalculationEnabled(const bool enabled = true) { this->en = enabled; }
		};
	}
}

#endif /* RFM12_AFC_H_ */