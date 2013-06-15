/*
 * CommandMask.h
 *
 * Created: 15.06.2013 21:08:22
 *  Author: Markus
 */ 


#ifndef COMMANDMASK_H_
#define COMMANDMASK_H_

#include <stdint.h>

namespace rfm12 
{
	namespace commands
	{
		/**
		* \brief Enumeration of all commands masks
		*/
		typedef enum {
			/**
			* \brief Configuration Setting Command (#1)
			*/
			RFM12MASK_CONFIGURATION_SETTING = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief Power Management Command (#2)
			*/
			RFM12MASK_POWERMANAGEMENT = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief Frequency Setting Command (#3)
			*/
			RFM12MASK_FREQUENCYSETTING = (uint16_t)0b1111000000000000U,
			
			/**
			* \brief Data Rate Command (#4)
			*/
			RFM12MASK_DATARATE = (uint16_t)0b1111111100000000U,
		
			/**
			* \brief Receiver Control Command (#5)
			*/
			RFM12MASK_RECEIVERCONTROL = (uint16_t)0b1111100000000000U,
			
			/**
			* \brief Data Filter Command (#6)
			*/
			RFM12MASK_DATAFILTER = (uint16_t)0b1111111100101000U,
			
			/**
			* \brief FIFO and Reset Mode Command (#7)
			*/
			RFM12MASK_FIFOANDRESETMODE = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief Synchron Pattern Command (#8)
			*/
			RFM12MASK_SYNCHRONPATTERN = (uint16_t)0b1111111111111111U,
			
			/**
			* \brief Receiver FIFO Read Command (#9)
			*/
			RFM12MASK_RECEIVERFIFO = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief AFC Command (#10)
			*/
			RFM12MASK_AFC = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief TX Configuration Control Command (#11)
			*/
			RFM12MASK_TXCONFIGURATION = (uint16_t)0b1111111000001000U,
			
			/**
			* \brief PLL Setting Command (#12)
			*/
			RFM12MASK_PLLSETTING = (uint16_t)0b1111111110010010U,
			
			/**
			* \brief Transmitter Register Write Command (#13)
			*/
			RFM12MASK_TRANSMITTERWRITE = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief Wake-Up Timer Command (#14)
			*/
			RFM12MASK_WAKEUPTIMER = (uint16_t)0b1110000000000000U,
			
			/**
			* \brief Configuration Setting Command (#15)
			*/
			RFM12MASK_LOWDUTYCYCLE = (uint16_t)0b1111111100000000U,
			
			/**
			* \brief Low Battery Detector and Microcontroller Clock Divider Command (#16)
			*/
			RFM12MASK_LOWBATTERY_MCCLOCKDIVDER = (uint16_t)0b1111111100010000U,
			
			/**
			* \brief Status Read Command (#17)
			*/
			RFM12MASK_STATUS_READ = (uint16_t)0b1111111111111111U,
		} commandmask_t;
	}
}



#endif /* COMMANDMASK_H_ */