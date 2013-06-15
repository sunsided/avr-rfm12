/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_TXWRITE_H_
#define RFM12_TXWRITE_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Transmit Register Write Command
		*/
		class TransmitRegisterWriteCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief Transmission data
					*/
					uint8_t				t:8;					
					
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
			TransmitRegisterWriteCommand()
				: command_word(0xB8AA)
			{}
				
			public:				

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		
			/**
			* \brief Sets the data for transmission.
			*/
			inline void setData(uint8_t byte = 0xAA) { this->t = byte; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_TRANSMITTERWRITE;
			}
		};
	}
}

#endif /* RFM12_TXWRITE_H_ */