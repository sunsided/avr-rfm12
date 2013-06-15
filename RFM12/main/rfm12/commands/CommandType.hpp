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
		* \brief The number of defined commands for the RFM12
		*/
		#define RFM12_COMMAND_COUNT (17U)
		
		/**
		* \brief Enumeration of all commands
		*/
		typedef enum {
			/**
			* \brief Configuration Setting Command
			*/
			RFM12CMD_CONFIGURATION_SETTING = 0,
			
			/**
			* \brief Power Management Command
			*/
			RFM12CMD_POWERMANAGEMENT = 1,
			
			/**
			* \brief Frequency Setting Command
			*/
			RFM12CMD_FREQUENCYSETTING = 2,
			
			/**
			* \brief Data Rate Command
			*/
			RFM12CMD_DATARATE = 3,
			
			/**
			* \brief Receiver Control Command
			*/
			RFM12CMD_RECEIVERCONTROL = 4,
			
			/**
			* \brief Data Filter Command
			*/
			RFM12CMD_DATAFILTER = 5,
			
			/**
			* \brief FIFO and Reset Mode Command
			*/
			RFM12CMD_FIFOANDRESETMODE = 6,
			
			/**
			* \brief Synchron Pattern Command
			*/
			RFM12CMD_SYNCHRONPATTERN = 7,
			
			/**
			* \brief Receiver FIFO Read Command
			*/
			RFM12CMD_RECEIVERFIFO = 8,
			
			/**
			* \brief AFC Command
			*/
			RFM12CMD_AFC = 9,
			
			/**
			* \brief TX Configuration Control Command
			*/
			RFM12CMD_TXCONFIGURATION = 10,
			
			/**
			* \brief PLL Setting Command
			*/
			RFM12CMD_PLLSETTING = 11,
			
			/**
			* \brief Transmitter Register Write Command
			*/
			RFM12CMD_TRANSMITTERWRITE = 12,
			
			/**
			* \brief Wake-Up Timer Command
			*/
			RFM12CMD_WAKEUPTIMER = 13,
			
			/**
			* \brief Configuration Setting Command
			*/
			RFM12CMD_LOWDUTYCYCLE = 14,
			
			/**
			* \brief Low Battery Detector and Microcontroller Clock Divider Command
			*/
			RFM12CMD_LOWBATTERY_MCCLOCKDIVDER = 15,
			
			/**
			* \brief Status Read Command
			*/
			RFM12CMD_STATUS_READ = 16,
		} commandtype_t;
	}
}



#endif /* COMMANDTYPE_H_ */