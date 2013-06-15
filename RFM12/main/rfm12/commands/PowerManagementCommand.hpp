/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_POWERMGMT_H_
#define RFM12_POWERMGMT_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Power Management Command
		*/
		class PowerManagementCommand : public Command 
		{
			friend class rfm12::Rfm12;
			
			protected:
			union {
				/**
				* \brief The raw command word.
				*/
				uint16_t command_word;
				struct {
					/**
					* \brief Disables the clock output (pin 8)
					*/
					bool				dc:1;
					
					/**
					* \brief Enables the wake-up timer
					*/
					bool				ew:1;
					
					/**
					* \brief Enables the low battery detector
					*/
					bool				eb:1;
					
					/**
					* \brief Turns on the crystal oscillator
					*/
					bool				ex:1;
					
					/**
					* \brief Turns on the synthesizer
					*/
					bool				es:1;
					
					/**
					* \brief Switches on the PLL, the power amplifier, and starts the transmission (If TX register is enabled)
					*/
					bool				et:1;

					/**
					* \brief The receiver baseband circuit can be separately switched on
					*/
					bool				ebb:1;
					
					/**
					* \brief Enables the whole receiver chain.
					*/
					bool				er:1;
																														
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
			PowerManagementCommand()
				: command_word(RFM12DEFAULT_POWERMANAGEMENT)
			{}
				
			public:				

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
				
			/**
			* \brief Sets whether the whole receiver chain shall be enabled.
			*
			* \param enabled Determines if the received chain shall be enabled or disabled.
			*/
			inline void setReceiverChainEnabled(const bool enabled = true) { this->er = enabled; }
				
			/**
			* \brief Sets whether the receiver baseband circuitry shall be enabled.
			*
			* The receiver baseband circuit can be separately switched on
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setReceiverBasebandCircuitryEnabled(const bool enabled = true) { this->ebb = enabled; }
				
			/**
			* \brief Switches on the PLL, the power amplifier, and starts the transmission (If TX register is enabled)
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setTransmissionEnabled(const bool enabled = true) { this->et = enabled; }
				
			/**
			* \brief Turns on the synthesizer
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setSynthesizerEnabled(const bool enabled = true) { this->es = enabled; }
				
			/**
			* \brief Turns on the crystal oscillator
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setCrystalOscillatorEnabled(const bool enabled = true) { this->ex = enabled; }
				
			/**
			* \brief Enables the low battery detector
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setLowBatteryDetectorEnabled(const bool enabled = true) { this->eb = enabled; }
			
			/**
			* \brief Enables the wake-up timer
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setWakeupTimerEnabled(const bool enabled = true) { this->ew = enabled; }
				
			/**
			* \brief Enables the clock output
			*
			* \param enabled true for enabled, false for disabled
			*/
			inline void setClockOutputEnabled(const bool enabled = true) { this->dc = !enabled; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_POWERMANAGEMENT;
			}
			
			
			/**
			* \brief Applies the command word if the mask matches.
			*
			* \param value The value to set
			* \returns true if the mask matched and the value was set, false otherwise
			*/
			inline bool applyCommandWord(const uint16_t value) {
				if (isMatch(value, RFM12MASK_POWERMANAGEMENT, RFM12DEFAULT_POWERMANAGEMENT)) {
					command_word = value;
					return true;
				}
				return false;
			}
		};
	}
}

#endif /* RFM12_POWERMGMT_H_ */