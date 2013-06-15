/* 
* ICommand.h
*
* Created: 10.06.2013 23:55:40
* Author: Markus
*/


#ifndef __ICommand_H__
#define __ICommand_H__

#include <stdint.h>
#include "CommandType.hpp"

namespace rfm12 
{
	namespace commands
	{
		class CommandResult;
		
		/**
		* \brief Interface for commands
		*/
		class Command
		{
		
		public:
			/**
			* Virtual destructor
			*/
			inline Command() {}
		
			/**
			* Virtual destructor
			*/
			inline virtual ~Command(){}
				
			/**
			* \brief Gets the command word.
			*/
			inline uint16_t getCommandWord() const { return (uint16_t)*this; }

			/**
			* \brief Cast operator
			*/
			virtual operator uint16_t() const = 0;

			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			virtual commandtype_t getCommandType() const = 0;
		};

	}
}

#endif //__ICommand_H__
