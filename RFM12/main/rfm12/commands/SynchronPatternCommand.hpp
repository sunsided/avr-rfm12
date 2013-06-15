/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_SYNCHRONPATTERN_H_
#define RFM12_SYNCHRONPATTERN_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Synchron Pattern Command
		*/
		class SynchronPatternCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief Synchron pattern byte zero.
					*
					* For RFM12 (i.e. not RFM12B), this value must always be 0xD4.
					*/
					uint8_t				b:8;
					
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
			SynchronPatternCommand()
				: command_word(0xCED4)
			{}

			public:

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the synchron pattern byte.
			*
			* For RFM12 (i.e. not RFM12B), this value must always be 0xD4.
			*/
			inline void setSynchronByte(const uint8_t byte = 0xD4) { this->b = byte; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_SYNCHRONPATTERN;
			}
		};
	}
}

#endif /* RFM12_SYNCHRONPATTERN_H_ */