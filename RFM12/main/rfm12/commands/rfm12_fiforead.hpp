/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_FIFOREAD_H_
#define RFM12_FIFOREAD_H_

#include <stdint.h>

namespace rfm12
{
	namespace commands
	{

		/**
		* \brief FIFO Read Command
		*/
		typedef class _rfm12_fiforead_command_t {
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief The command code.
					*/
					const uint8_t		command_code:8;		

					/**
					* \brief Padding.
					*/
					const uint8_t		:8;		
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			_rfm12_fiforead_command_t() 
				: command_word(0xB000)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

		} Rfm12FifoReadCommand;

	}
}

#endif /* RFM12_FIFOREAD_H_ */