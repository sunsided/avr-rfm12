/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_FREQUENCY_H_
#define RFM12_FREQUENCY_H_

#include <stdlib.h>
#include <stdint.h>
#include "ICommand.hpp"
#include "ConfigSetCommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Frequency Setting Command
		*/
		class FrequencyCommand : public ICommand {
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
					const uint8_t		command_code:4;

					/**
					* \brief The frequency parameter F.
					*/
					uint16_t			f:12;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			FrequencyCommand() 
				: command_word(0xA680)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the target frequency by frequency.
			*
			* \see set_by_channel()
			*
			* \param frequency		The target frequency.
			* \param real_frequency Optional. The real calculated target frequency.
			* \return true in case of success, false if the frequency was out of range
			*/
			bool setByFrequency(const float frequency = 433.0F, float *real_frequency = NULL);

			/**
			* \brief Sets the target frequency by channel.
			*
			* \see set_by_frequency()
			*
			* \param channel		The target channel.
			* \param band			The frequency band.
			* \param real_frequency Optional. The real calculated target frequency. If no valid band was set, this value will be set to zero.
			* \return true in case of success, false if the channel was out of range
			*/
			bool setByChannel(const uint16_t channel = 1200, const frequency_band_t band = FREQ_BAND_433, float *real_frequency = NULL);
		};
	}
}

#endif /* RFM12_FREQUENCY_H_ */