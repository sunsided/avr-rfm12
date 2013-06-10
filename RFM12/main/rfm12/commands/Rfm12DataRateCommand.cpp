/*
 * rfm12_datarate.cpp
 *
 * Created: 06.06.2013 12:06:58
 *  Author: Markus
 */ 

#include <tgmath.h>
#include <float.h>
#include "Rfm12DataRateCommand.hpp"

using namespace rfm12::commands;

/**
* \brief Sets the bit rate.
*
* Sets the actual bit rate in transmit mode and the expected bit rate in receive mode.
*
* \param bitrate The bitrate in kbps (in range of ]0 to 256.0])
*/
bool Rfm12DataRateCommand::setBitRate(const float bitrate, float* real_bitrate) 
{
	bool valid = true;
	
	// sanity check
	if (bitrate <= 0 + FLT_EPSILON) {
		if (NULL != real_bitrate) {
			*real_bitrate = 0.0F;
		}
		return false;
	}
	
	// R is a 7-bit value (range of 0..127)

	// Expected bit rate:
	// BR = 10000/29/(R+1)/(1+CS*7) [kbps]

	// Receiver bit rate:
	// R = (10000/29/(1+cs*7)/BR) -1
	
	// According to MATLAB:
	// r = solve('10000/29/(r+1)/(1+cs*7)=br', 'r')
	// r = 1/(br*((203*cs)/10000 + 29/10000)) - 1
	
	// e.g. for br = 0.6 kbps
	// cs = 0			1/(0.6*(29/10000)) - 1						573.7126436781610		invalid (>127)
	// cs = 1			1/(0.6*(203/10000 + 29/10000)) - 1			70.839080459770130		71
	
	// e.g. for br = 49.2 kbps
	// cs = 0			1/(49.2*(29/10000)) - 1						6.008690776562938		6
	// cs = 1			1/(49.2*(203/10000 + 29/10000)) - 1			-0.123913652929633		invalid (<0)
	
	// e.g. for br = 115.2 kbps
	// cs = 0			1/(115.2*(29/10000)) - 1					1.993295019157088		2
	// cs = 1			1/(115.2*(203/10000 + 29/10000)) - 1		-0.625838122605364		invalid (<0)
	
	// e.g. for br = 256.0 kbps
	// cs = 0			1/(256.0*(29/10000)) - 1					0.301236174365648		0
	// cs = 1			1/(256.0*(203/10000 + 29/10000)) - 1		-0.837345478204294		invalid
	
	// calculate for cs = 0
	uint8_t cs = 0;
	float r_flt = 10000.0F/(bitrate*29.0F) - 1.0F;
	
	// sanity check
	if (r_flt <= 0 + FLT_EPSILON || r_flt >= 127.0 + FLT_EPSILON) {
		// calculate for cs = 1
		r_flt = 1.0F/(bitrate*232.0F/10000.0F) - 1.0F;
		cs = 1;
		
		// sanity check
		if (r_flt <= 0 + FLT_EPSILON || r_flt >= 127.0 + FLT_EPSILON) {
			valid = false;
		}
	}
	
	// Convert data
	if (valid) {
		uint8_t r = static_cast<uint8_t>(round(r_flt));
		if (r > 127) r = 127;
		
		this->cs = cs;
		this->r = r;
		
		if (NULL != real_bitrate) {
			*real_bitrate = 10000.0F / 29.0F / (r+1) / (1+cs*7);
		}
	}
	else {
		if (NULL != real_bitrate) {
			*real_bitrate = 0.0F;
		}
	}
	return valid;
}
