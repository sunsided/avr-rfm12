/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_DATARATE_H_
#define RFM12_DATARATE_H_

#include <stdlib.h>
#include <stdint.h>
#include "ICommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Data Rate Command
		*/
		class DataRateCommand : public ICommand {
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
					* \brief Expected bit rate CS value
					*/
					uint8_t				cs:1;
			
					/**
					* \brief Expected bit rate R values
					*/
					uint8_t				r:7;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			DataRateCommand() 
				: command_word(0xC623)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the bit rate.
			*
			* Sets the actual bit rate in transmit mode and the expected bit rate in receive mode.
			*
			* \param bitrate The bitrate in kbps (in range of 0 to 256.0)
			* \param real_bitrate Optional. The real calculated bitrate.
			*/
			bool setBitRate(const float bitrate = 115.20F, float* real_bitrate = NULL);
		};
	}
}

#endif /* RFM12_DATARATE_H_ */