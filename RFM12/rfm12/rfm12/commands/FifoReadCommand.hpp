/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_FIFOREAD_H_
#define RFM12_FIFOREAD_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief FIFO Read Command
		*/
		class FifoReadCommand : public Command {
			friend class rfm12::Rfm12;
			
			protected:
			union {
				/**
				* \brief The raw command word.
				*/
				uint16_t command_word;
				struct {
					/**
					* \brief Padding.
					*/
					const uint8_t		:8;
					
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
			inline FifoReadCommand()
				: command_word(RFM12DEFAULT_RECEIVERFIFO)
			{}
				
			/**
			* \brief Initializes this instance from another instance
			*/
			inline FifoReadCommand(const FifoReadCommand& other)
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
				return RFM12CMD_RECEIVERFIFO;
			}
			
			/**
			* \brief Applies the command word if the mask matches.
			*
			* \param value The value to set
			* \returns true if the mask matched and the value was set, false otherwise
			*/
			inline bool applyCommandWord(const uint16_t value) {
				if (isMatch(value, RFM12MASK_RECEIVERFIFO, RFM12DEFAULT_RECEIVERFIFO)) {
					command_word = value;
					return true;
				}
				return false;
			}
		};
	}
}

#endif /* RFM12_FIFOREAD_H_ */