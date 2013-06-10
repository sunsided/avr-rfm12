/*
 * rfm12_frequency.cpp
 *
 * Created: 06.06.2013 11:24:45
 *  Author: Markus
 */ 

#include <tgmath.h>
#include <float.h>
#include "Rfm12FrequencyCommand.hpp"

using namespace rfm12::commands;

/**
* \brief Sets the target frequency by frequency.
*
* \see set_by_channel()
*
* \param frequency The target frequency.
* \param real_frequency The real calculated target frequency.
* \return true in case of success, false if the frequency was out of range
*/
bool Rfm12FrequencyCommand::setByFrequency(const float frequency, float *real_frequency)
{
	// f0 = 10*c1*(c2 + frequency/4000)
	// F  = (400*(frequency - 10*c1*c2))
	
	bool valid = true;
	uint16_t F;
	
	if ((frequency >= (430.24 - FLT_EPSILON)) && (frequency <= (439.7575 + FLT_EPSILON)))
	{
		// 433MHz band: c1 = 1, c2 = 43
		F = static_cast<uint16_t>(floor((400.0F*(frequency - 430.0F))));
		
		// sanity check
		if (F < 96) {
			F = 96;
			valid = false;
		}
		else if (F > 3903) {
			F = 3903;
			valid = false;
		}
		
		// calculate real frequency
		if (NULL != real_frequency) {
			*real_frequency = 10.0F*(43.0F+F/4000.0F);
		}
	}
	else if ((frequency >= (860.48 - FLT_EPSILON)) && (frequency <= (879.515 + FLT_EPSILON)))
	{
		// 868MHz band: c1 = 2, c2 = 43
		F = static_cast<uint16_t>(floor((200.0F*(frequency - 860.0F))));
		
		// sanity check
		if (F < 96) {
			F = 96;
			valid = false;
		}
		else if (F > 3903) {
			F = 3903;
			valid = false;
		}
		
		// calculate real frequency
		if (NULL != real_frequency) {
			*real_frequency = 20.0F*(43.0F+F/4000.0F);
		}
	}
	else if ((frequency >= (900.72 - FLT_EPSILON)) && (frequency <= (929.2725 + FLT_EPSILON)))
	{
		// 915MHz band: c1 = 3, c2 = 30
		F = static_cast<uint16_t>(floor((400.0F*(frequency - 900.0F))/3.0F));
		
		// sanity check
		if (F < 96) {
			F = 96;
			valid = false;
		}
		else if (F > 3903) {
			F = 3903;
			valid = false;
		}
		
		// calculate real frequency
		if (NULL != real_frequency) {
			*real_frequency = 30.0F*(30.0F+F/4000.0F);
		}
	}
	else {
		valid = false;
		
		// calculate real frequency
		if (NULL != real_frequency) {
			*real_frequency = 0.0F;
		}
	}
	
	// Set values if successful
	if (valid) {
		this->f = F;
		return true;
	}
	return false;
}

/**
* \brief Sets the target frequency by channel.
*
* \see set_by_frequency()
*
* \param channel The target channel.
* \param real_frequency The real calculated target frequency.
* \return true in case of success, false if the frequency was out of range
*/
bool Rfm12FrequencyCommand::setByChannel(const uint16_t channel, const frequency_band_t band, float *real_frequency)
{
	// f0 = 10*c1*(c2 + frequency/4000)
	// F  = (400*(frequency - 10*c1*c2))
	
	bool valid = true;
	uint16_t F = channel;
	
	// sanity check
	if (channel < 96) {
		F = 96;
		valid = false;
	}
	else if (channel > 3903) {
		F = 3903;
		valid = false;
	}
	
	// select the band
	switch (band) 
	{
		case FREQ_BAND_433: 
		{
			// 433MHz band: c1 = 1, c2 = 43
			
			// calculate real frequency
			if (NULL != real_frequency) {
				*real_frequency = 10.0F*(43.0F+F/4000.0F);
			}
			
			break;
		}
		case FREQ_BAND_868: 
		{
			// 868MHz band: c1 = 2, c2 = 43
			
			// calculate real frequency
			if (NULL != real_frequency) {
				*real_frequency = 20.0F*(43.0F+F/4000.0F);
			}
			
			break;
		}
		case FREQ_BAND_915: 
		{
			// 915MHz band: c1 = 3, c2 = 30
			
			// calculate real frequency
			if (NULL != real_frequency) {
				*real_frequency = 30.0F*(30.0F+F/4000.0F);
			}
			
			break;
		}
		default:
		{
			valid = false;
			if (NULL != real_frequency) {
				*real_frequency = 0.0F;
			}
		}
	}
	
	// Set values if successful
	if (valid) {
		this->f = F;
		return true;
	}
	return false;
}