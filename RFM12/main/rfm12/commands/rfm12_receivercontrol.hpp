/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_RECEIVERCONTROL_H_
#define RFM12_RECEIVERCONTROL_H_

#include <stdint.h>
#include "../IRfm12Command.h"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Pin 16 Mode
		*/
		typedef enum {
			PIN16_INTERRUPT_IN = 0b0,
			PIN16_VDI_OUT = 0b1
		} pin16_t;

		/**
		* \brief Pin 16 Mode
		*/
		typedef enum {
			VDI_FAST	= 0b00,			//<! fast VDI response
			VDI_MEDIUM	= 0b01,			//<! medium VDI response
			VDI_SLOW	= 0b10,			//<! slow VDI response
			VDI_ALWAYS	= 0b11			//<! VDI always on
		} vdi_response_t;

		/**
		* \brief Receiver Baseband Bandwidth
		*/
		typedef enum {
			RBBW_RESERVED	= 0b000,	//<! reserved
			RBBW_400		= 0b001,	//<! 400 kHz
			RBBW_340		= 0b010,	//<! 340 kHz
			RBBW_270		= 0b011,	//<! 270 kHz
			RBBW_200		= 0b100,	//<! 200 kHz
			RBBW_134		= 0b101,	//<! 134 kHz
			RBBW_67			= 0b110,	//<! 67 kHz
			RBBW_RESERVED_2 = 0b111,	//<! reserved
		} baseband_bandwidth_t;

		/**
		* \brief Negative LNA gain factor relative to maximum
		*/
		typedef enum {
			LNGAIN_0		= 0b00,		//<! -0 dB
			LNGAIN_6		= 0b01,		//<! -6 dB
			LNGAIN_14		= 0b10,		//<! -14 dB
			LNGAIN_20		= 0b11,		//<! -20 dB
		} lna_gain_t;

		/**
		* \brief RSSI detector threshold
		*/
		typedef enum {
			RSSI_103		= 0b000,		//<! -103 dB
			RSSI_97			= 0b001,		//<! -97 dB
			RSSI_91			= 0b010,		//<! -91 dB
			RSSI_85			= 0b011,		//<! -85 dB
			RSSI_79			= 0b100,		//<! -79 dB
			RSSI_73			= 0b101,		//<! -73 dB
			RSSI_RESERVED	= 0b110,		//<! reserved
			RSSI_RESERVED_2	= 0b111,		//<! reserved
		} rssi_threshold_t;

		/**
		* \brief Receiver Control Command
		*/
		typedef class _rfm12_receivercontrol_command_t : public IRfm12Command {
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
					const uint8_t			command_code:5;		

					/**
					* \brief Pin 16 Function Select
					*/
					pin16_t					p16:1;
			
					/**
					* \brief VDI (valid data indicator) signal response time setting.
					*/
					vdi_response_t			d:2;
			
					/**
					* \brief Receiver baseband bandwidth (BW) select
					*/
					baseband_bandwidth_t	i:3;
			
					/**
					* \brief LNA gain select:
					*/
					lna_gain_t				g:2;
			
					/**
					* \brief RSSI detector threshold base setting.
					*
					* Real RSSI threshold depends on LNA gain setting.
					*/
					rssi_threshold_t		r:3;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			_rfm12_receivercontrol_command_t() 
				: command_word(0x9080)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

		} Rfm12ReceiverControlCommand;

	}
}

#endif /* RFM12_RECEIVERCONTROL_H_ */