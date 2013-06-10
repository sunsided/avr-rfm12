/* 
* IRfm12Command.h
*
* Created: 10.06.2013 23:55:40
* Author: Markus
*/


#ifndef __IRFM12COMMAND_H__
#define __IRFM12COMMAND_H__

#include <stdint.h>

namespace rfm12 
{
	namespace commands
	{
		/**
		* \brief Interface for commands
		*/
		class IRfm12Command
		{
		public:
			virtual ~IRfm12Command(){}
				
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

#endif //__IRFM12COMMAND_H__
