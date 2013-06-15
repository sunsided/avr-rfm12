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
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
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
			FifoReadCommand(Rfm12 *owner) 
				: Command(owner), command_word(0xB000)
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
		};
	}
}

#endif /* RFM12_FIFOREAD_H_ */