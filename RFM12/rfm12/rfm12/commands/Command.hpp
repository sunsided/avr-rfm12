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
#include "CommandMask.hpp"
#include "CommandDefaults.hpp"

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
			
			/**
			* \brief Applies the command word if the mask matches.
			*
			* \param value The value to set
			* \returns true if the mask matched and the value was set, false otherwise
			*/
			virtual bool applyCommandWord(const uint16_t value) = 0;
			
			/**
			* \brief Compares a command word with the command prototype.
			*
			* \param commandword The command word
			* \param mask The command code mask
			* \param prototype The command prototype (e.g. default value, empty command, ...)
			* \returns true if commandword is a valid command according to mask and prototype, false otherwise
			*/	
			static inline const bool isMatch(const uint16_t commandword, const commandmask_t mask, const commanddefault_t prototype) {
				return ((commandword & mask) ^ (prototype & mask)) == 0;
			}
		};

	}
}

#endif //__ICommand_H__
