/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_DATAFILTER_H_
#define RFM12_DATAFILTER_H_

#include <stdint.h>
#include "ICommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Data Filter Type
		*/
		typedef enum {
			FILTER_DIGITAL = 0,		//<! Digital filter
			FILTER_ANALOG = 1		//<! Analog RC filter
		} filter_type_t;

		/**
		* \brief Data Filter Command
		*/
		class DataFilterCommand : public ICommand {
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief DQD threshold parameter.
					*
					* Determines the threshold level at which the DQD signal goes high. The 
					* valid range is 0 to 7, but the value should always be larger than or equal to 4 (the default).
					* It can be calculated using the following formula:
					*
					* f = 4 x (deviation – TX-RXoffset ) / bit rate
					*/
					uint8_t				f:3;
					
					/**
					* \brief Padding bit
					*/
					const uint8_t		:1;
					
					/**
					* \brief Filter Type
					*/
					filter_type_t		s:1;
					
					/**
					* \brief Padding bit
					*/
					const uint8_t		:1;
					
					/**
					* \brief Clock recovery lock control
					*/
					uint8_t				ml:1;
					
					/**
					* \brief Clock recovery (CR) auto lock enable
					*/
					bool				al:1;
					
					/**
					* \brief The command code.
					*/
					const uint8_t		command_code:8;		
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			DataFilterCommand () 
				: command_word(0xC22C)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		};
	}
}
#endif /* RFM12_DATAFILTER_H_ */