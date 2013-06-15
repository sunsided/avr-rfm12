/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_RECEIVERCONTROL_H_
#define RFM12_RECEIVERCONTROL_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
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
		class ReceiverControlCommand : public Command {
			friend class rfm12::Rfm12;
			
			protected:
			union {
				/**
				* \brief The raw command word.
				*/
				uint16_t command_word;
				struct {
					/**
					* \brief RSSI detector threshold base setting.
					*
					* Real RSSI threshold depends on LNA gain setting.
					*/
					rssi_threshold_t		r:3;
					
					/**
					* \brief LNA gain select:
					*/
					lna_gain_t				g:2;
					
					/**
					* \brief Receiver baseband bandwidth (BW) select
					*/
					baseband_bandwidth_t	i:3;
					
					/**
					* \brief VDI (valid data indicator) signal response time setting.
					*/
					vdi_response_t			d:2;
					
					/**
					* \brief Pin 16 Function Select
					*/
					pin16_t					p16:1;
					
					/**
					* \brief The command code.
					*/
					const uint8_t			command_code:5;		
				};
			};
	
			private:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			ReceiverControlCommand()
				: command_word(RFM12DEFAULT_RECEIVERCONTROL)
			{}

			public:

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
				
			/*
			* \brief Sets operation mode of Pin 16
			*
			* \param mode The mode
			*/
			inline void setPin16Mode(const pin16_t mode = PIN16_INTERRUPT_IN) { this->p16 = mode; }
				
			/*
			* \brief Sets the VDI response time
			*
			* \param value
			*/
			inline void setVdiResponseTime(const vdi_response_t value = VDI_FAST) { this->d = value; }
				
			/*
			* \brief Sets the receiver baseband bandwidth
			*
			* \param value
			*/
			inline void setReceiverBasebandBandwidth(const baseband_bandwidth_t value = RBBW_200) { this->i = value; }
				
			/*
			* \brief Sets the LNA gain
			*
			* \param value
			*/
			inline void setLnaGain(const lna_gain_t value = LNGAIN_0) { this->g = value; }
				
			/*
			* \brief Sets the RSSI detector threshold
			*
			* \param value
			*/
			inline void setRssiDetectorThreshold(const rssi_threshold_t value = RSSI_103) { this->r = value; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_RECEIVERCONTROL;
			}
			
			/**
			* \brief Applies the command word if the mask matches.
			*
			* \param value The value to set
			* \returns true if the mask matched and the value was set, false otherwise
			*/
			inline bool applyCommandWord(const uint16_t value) {
				if (isMatch(value, RFM12MASK_RECEIVERCONTROL, RFM12DEFAULT_RECEIVERCONTROL)) {
					command_word = value;
					return true;
				}
				return false;
			}
		};
	}
}

#endif /* RFM12_RECEIVERCONTROL_H_ */