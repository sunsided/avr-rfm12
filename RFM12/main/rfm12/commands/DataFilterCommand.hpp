/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_DATAFILTER_H_
#define RFM12_DATAFILTER_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Data Filter Type
		*/
		typedef enum {
			FILTER_DIGITAL = 0,		//<! Digital filter
			FILTER_ANALOG = 1		//<! Analog RC filter
		} filter_type_t;

		/**
		* \brief Data Filter Command
		*/
		class DataFilterCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief DQD threshold parameter.
					*
					* Determines the threshold level at which the DQD signal goes high. The 
					* valid range is 0 to 7, but the value should always be larger than or equal to 4 (the default).
					* It can be calculated using the following formula:
					*
					* f = 4 x (deviation – TX-RXoffset ) / bit rate
					*/
					uint8_t				f:3;
					
					/**
					* \brief Padding bit
					*/
					const uint8_t		:1;
					
					/**
					* \brief Filter Type
					*/
					filter_type_t		s:1;
					
					/**
					* \brief Padding bit
					*/
					const uint8_t		:1;
					
					/**
					* \brief Clock recovery lock control
					*/
					uint8_t				ml:1;
					
					/**
					* \brief Clock recovery (CR) auto lock enable
					*/
					bool				al:1;
					
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
			DataFilterCommand (Rfm12 *owner) 
				: Command(owner), command_word(0xC22C)
			{}

			public:

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
				
			/**
			* \brief Clock recovery (CR) auto lock control
			*
			* \param enabled auto mode if true, false otherwise
			*/
			inline void setClockRecoveryAutoLockModeEnabled(const bool enabled = true) { this->al = enabled; }
				
			/**
			* \brief Clock recovery lock control
			*
			* \param enabled fast mode if true, slow mode otherwise
			*/
			inline void setClockRecoveryFastLockEnabled(const bool enabled = true) { this->ml = enabled; }
				
			/**
			* \brief Sets the data filter type.
			*
			* \param value The filter type
			*/
			inline void setDataFilterType(const filter_type_t value = FILTER_DIGITAL) { this->s = value; }
				
			/**
			* \brief DQD threshold parameter.
			*
			* Determines the threshold level at which the DQD signal goes high. The
			* valid range is 0 to 7, but the value should always be larger than or equal to 4 (the default).
			* It can be calculated using the following formula:
			*
			* f = 4 x (deviation – TX-RXoffset ) / bit rate
			*
			* \param value The DQD threshold value (in range of 0 to 7).
			*/
			inline void setDqdTrheshold(const uint8_t value = 0b100) { this->f = (value < 7 ? value : 7); }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_DATAFILTER;
			}
		};
	}
}
#endif /* RFM12_DATAFILTER_H_ */