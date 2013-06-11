/* 
* ICommand.h
*
* Created: 10.06.2013 23:55:40
* Author: Markus
*/


#ifndef __ICommand_H__
#define __ICommand_H__

#include <stdint.h>

namespace rfm12 
{
	namespace commands
	{
		/**
		* \brief Interface for commands
		*/
		class ICommand
		{
		public:
			// inline virtual ~ICommand(){}
				
			/**
			* \brief Gets the command word.
			*/
			inline uint16_t getCommandWord() const { return (uint16_t)*this; }

			/**
			* \brief Cast operator
			*/
			virtual operator uint16_t() const = 0;

		};

	}
}

#endif //__ICommand_H__
