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
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Data Rate Command
		*/
		class DataRateCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief Expected bit rate R values
					*/
					uint8_t				r:7;
					
					/**
					* \brief Expected bit rate CS value
					*/
					uint8_t				cs:1;
					
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
			DataRateCommand(Rfm12 *owner) 
				: Command(owner), command_word(0xC623)
			{}

			public:

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
			
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_DATARATE;
			}
		};
	}
}

#endif /* RFM12_DATARATE_H_ */