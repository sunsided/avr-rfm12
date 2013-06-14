/*
 * CommandType.hpp
 *
 * Created: 14.06.2013 20:16:55
 *  Author: Markus
 */ 


#ifndef COMMANDTYPE_H_
#define COMMANDTYPE_H_

namespace rfm12 
{
	namespace commands
	{
		/**
		* \brief Enumeration of all commands
		*/
		typedef enum {
			/**
			* \brief Configuration Setting Command
			*/
			RFM12CMD_CONFIGURATION_SETTING = 1,
			
			/**
			* \brief Power Management Command
			*/
			RFM12CMD_POWERMANAGEMENT = 2,
			
			/**
			* \brief Frequency Setting Command
			*/
			RFM12CMD_FREQUENCYSETTING = 3,
			
			/**
			* \brief Data Rate Command
			*/
			RFM12CMD_DATARATE = 4,
			
			/**
			* \brief Receiver Control Command
			*/
			RFM12CMD_RECEIVERCONTROL = 5,
			
			/**
			* \brief Data Filter Command
			*/
			RFM12CMD_DATAFILTER = 6,
			
			/**
			* \brief FIFO and Reset Mode Command
			*/
			RFM12CMD_FIFOANDRESETMODE = 7,
			
			/**
			* \brief Synchron Pattern Command
			*/
			RFM12CMD_SYNCHRONPATTERN = 8,
			
			/**
			* \brief Receiver FIFO Read Command
			*/
			RFM12CMD_RECEIVERFIFO = 9,
			
			/**
			* \brief AFC Command
			*/
			RFM12CMD_AFC = 10,
			
			/**
			* \brief TX Configuration Control Command
			*/
			RFM12CMD_TXCONFIGURATION = 11,
			
			/**
			* \brief PLL Setting Command
			*/
			RFM12CMD_PLLSETTING = 12,
			
			/**
			* \brief Transmitter Register Write Command
			*/
			RFM12CMD_TRANSMITTERWRITE = 13,
			
			/**
			* \brief Wake-Up Timer Command
			*/
			RFM12CMD_WAKEUPTIMER = 14,
			
			/**
			* \brief Configuration Setting Command
			*/
			RFM12CMD_LOWDUTYCYCLE = 15,
			
			/**
			* \brief Low Battery Detector and Microcontroller Clock Divider Command
			*/
			RFM12CMD_LOWBATTERY_MCCLOCKDIVDER = 16,
			
			/**
			* \brief Status Read Command
			*/
			RFM12CMD_STATUS_READ = 17,
		} commandtype_t;
	}
}



#endif /* COMMANDTYPE_H_ */