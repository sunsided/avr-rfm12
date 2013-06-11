/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_STATUSREAD_H_
#define RFM12_STATUSREAD_H_

#include <stdint.h>
#include "ICommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Status Read Command
		*/
		class StatusReadCommand : public ICommand {
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
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
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			StatusReadCommand() 
				: command_word(0x0000)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		};
	}
}

#endif /* RFM12_STATUSREAD_H_ */