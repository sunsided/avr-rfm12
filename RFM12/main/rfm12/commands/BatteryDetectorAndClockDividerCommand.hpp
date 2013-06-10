/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_BATTERYCLOCKDIVIDER_H_
#define RFM12_BATTERYCLOCKDIVIDER_H_

#include <stdint.h>
#include "ICommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Clock Output Frequency
		*/
		typedef enum {
			CLKOUTFREQ_1000kHZ,				//<! 1 MHz
			CLKOUTFREQ_1250kHZ,				//<! 1.25 MHz
			CLKOUTFREQ_1660kHZ,				//<! 1.66 MHz
			CLKOUTFREQ_2000kHZ,				//<! 2 MHz
			CLKOUTFREQ_2500kHZ,				//<! 2.5 MHz
			CLKOUTFREQ_3330kHZ,				//<! 3.33 MHz
			CLKOUTFREQ_5000kHZ,				//<! 5 MHz
			CLKOUTFREQ_10000kHZ,			//<! 10 MHz
		} clockout_freq_t;

		/**
		* \brief Battery threshold setting
		*/
		typedef enum {
			BATTHRESH_2250mV = 0b0000,		//<! 2.25 V
			BATTHRESH_2350mV = 0b0001,		//<! 2.35 V
			BATTHRESH_2450mV = 0b0010,		//<! 2.45 V
			BATTHRESH_2550mV = 0b0011,		//<! 2.55 V
			BATTHRESH_2650mV = 0b0100,		//<! 2.65 V
			BATTHRESH_2750mV = 0b0101,		//<! 2.75 V
			BATTHRESH_2850mV = 0b0110,		//<! 2.85 V
			BATTHRESH_2950mV = 0b0111,		//<! 2.95 V
	
			BATTHRESH_3050mV = 0b1000,		//<! 3.05 V
			BATTHRESH_3150mV = 0b1001,		//<! 3.15 V
			BATTHRESH_3250mV = 0b1010,		//<! 3.25 V
			BATTHRESH_3350mV = 0b1011,		//<! 3.35 V
			BATTHRESH_3450mV = 0b1100,		//<! 3.45 V
			BATTHRESH_3550mV = 0b1101,		//<! 3.55 V
			BATTHRESH_3650mV = 0b1110,		//<! 3.65 V
			BATTHRESH_3750mV = 0b1111,		//<! 3.75 V
		} battery_threshold_t;

		/**
		* \brief Low Battery Detector and Microcontroller Clock Divider Command
		*/
		class BatteryDetectorAndClockDividerCommand : public ICommand 
		{
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
					* \brief Clock divider configuration.
					*
					* Controls the clock output frequency.
					*/
					clockout_freq_t		d:3;
			
					/**
					* \brief Padding
					*/
					const uint8_t		:1;
			
					/**
					* \brief Battery threshold voltage.
					*
					* V = 2.25 + V · 0.1 [V]
					*/
					battery_threshold_t	v:4;
			
					/**
					* \brief Padding
					*/
					const uint8_t		:1;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			BatteryDetectorAndClockDividerCommand() 
				: command_word(0xC000)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		};
	}
}

#endif /* RFM12_BATTERYCLOCKDIVIDER_H_ */