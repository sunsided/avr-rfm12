/*
 * CommandDefaults.hpp
 *
 * Created: 15.06.2013 21:24:20
 *  Author: Markus
 */ 


#ifndef COMMANDDEFAULTS_H_
#define COMMANDDEFAULTS_H_

namespace rfm12 
{
	namespace commands
	{	
		/**
		* \brief Enumeration of all command default values
		*/
		typedef enum {
			/**
			* \brief Configuration Setting Command (#1)
			*/
			RFM12DEFAULT_CONFIGURATION_SETTING = 0x8008U,
			
			/**
			* \brief Power Management Command (#2)
			*/
			RFM12DEFAULT_POWERMANAGEMENT = (uint16_t)0x8208U,
			
			/**
			* \brief Frequency Setting Command (#3)
			*/
			RFM12DEFAULT_FREQUENCYSETTING = (uint16_t)0xA680U,
			
			/**
			* \brief Data Rate Command (#4)
			*/
			RFM12DEFAULT_DATARATE = (uint16_t)0xC623U,
		
			/**
			* \brief Receiver Control Command (#5)
			*/
			RFM12DEFAULT_RECEIVERCONTROL = (uint16_t)0x9080U,
			
			/**
			* \brief Data Filter Command (#6)
			*/
			RFM12DEFAULT_DATAFILTER = (uint16_t)0xC22CU,
			
			/**
			* \brief FIFO and Reset Mode Command (#7)
			*/
			RFM12DEFAULT_FIFOANDRESETMODE = (uint16_t)0xCA80U,
			
			/**
			* \brief Synchron Pattern Command (#8)
			*/
			RFM12DEFAULT_SYNCHRONPATTERN = (uint16_t)0xCED4U,
			
			/**
			* \brief Receiver FIFO Read Command (#9)
			*/
			RFM12DEFAULT_RECEIVERFIFO = (uint16_t)0xB000U,
			
			/**
			* \brief AFC Command (#10)
			*/
			RFM12DEFAULT_AFC = (uint16_t)0xC4F7U,
			
			/**
			* \brief TX Configuration Control Command (#11)
			*/
			RFM12DEFAULT_TXCONFIGURATION = (uint16_t)0x9800U,
			
			/**
			* \brief PLL Setting Command (#12)
			*/
			RFM12DEFAULT_PLLSETTING = (uint16_t)0xCC77U,
			
			/**
			* \brief Transmitter Register Write Command (#13)
			*/
			RFM12DEFAULT_TRANSMITTERWRITE = (uint16_t)0xB8AAU,
			
			/**
			* \brief Wake-Up Timer Command (#14)
			*/
			RFM12DEFAULT_WAKEUPTIMER = (uint16_t)0xE196U,
			
			/**
			* \brief Configuration Setting Command (#15)
			*/
			RFM12DEFAULT_LOWDUTYCYCLE = (uint16_t)0xC80EU,
			
			/**
			* \brief Low Battery Detector and Microcontroller Clock Divider Command (#16)
			*/
			RFM12DEFAULT_LOWBATTERY_MCCLOCKDIVDER = (uint16_t)0xC000U,
			
			/**
			* \brief Status Read Command (#17)
			*/
			RFM12DEFAULT_STATUS_READ = (uint16_t)0x0000U,
		} commanddefault_t;
	}
}


#endif /* COMMANDDEFAULTS_H_ */