/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_STATUSREAD_H_
#define RFM12_STATUSREAD_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Status Read Command
		*/
		class StatusReadCommand : public Command {
			friend class rfm12::Rfm12;
			
			protected:
			union {
				/**
				* \brief The raw command word.
				*/
				uint16_t command_word;
				struct {
					/**
					* \brief Padding
					*/
					const uint16_t		:15;
					
					/**
					* \brief The command code.
					*/
					const uint16_t		command_code:1;
				};
			};
	
			private:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			inline StatusReadCommand()
				: command_word(RFM12DEFAULT_STATUS_READ)
			{}
				
			/**
			* \brief Initializes this instance from another instance
			*/
			inline StatusReadCommand(const StatusReadCommand& other)
				: command_word(other.command_word)
			{}

			public:

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_STATUS_READ;
			}
			
			/**
			* \brief Applies the command word if the mask matches.
			*
			* \param value The value to set
			* \returns true if the mask matched and the value was set, false otherwise
			*/
			inline bool applyCommandWord(const uint16_t value) {
				if (isMatch(value, RFM12MASK_STATUS_READ, RFM12DEFAULT_STATUS_READ)) {
					command_word = value;
					return true;
				}
				return false;
			}
		};
	}
}

#endif /* RFM12_STATUSREAD_H_ */