/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_CONFIGSET_H_
#define RFM12_CONFIGSET_H_

#include <stdint.h>
#include "ICommand.hpp"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Frequency Band 
		* \see rfm12_configset_commant_t
		*/
		typedef enum {
			FREQ_BAND_RESERVED = 0b00,	//<! unused
			FREQ_BAND_433 = 0b01,		//<! 433 MHz
			FREQ_BAND_868 = 0b10,		//<! 868 MHz
			FREQ_BAND_915 = 0b11,		//<! 915 MHz
		} frequency_band_t;

		/**
		* \brief Crystal Load Capacitance
		* \see rfm12_configset_commant_t
		*/
		typedef enum {
			CL_085 = 0b0000,			//<!  8.5pF
			CL_090 = 0b0001,			//<!  9.0pF
			CL_095 = 0b0010,			//<!  9.5pF
			CL_100 = 0b0011,			//<! 10.0pF
			CL_105 = 0b0100,			//<! 10.5pF
			CL_110 = 0b0101,			//<! 11.0pF
			CL_115 = 0b0110,			//<! 11.5pF
			CL_120 = 0b0111,			//<! 12.0pF
			CL_125 = 0b1000,			//<! 12.5pF
			CL_130 = 0b1001,			//<! 13.0pF
			CL_135 = 0b1010,			//<! 13.5pF
			CL_140 = 0b1011,			//<! 14.0pF
			CL_145 = 0b1100,			//<! 14.5pF
			CL_150 = 0b1101,			//<! 15.0pF
			CL_155 = 0b1110,			//<! 15.5pF
			CL_160 = 0b1111,			//<! 16.0pF
		} crystal_load_t;

		/**
		* \brief Configuration Setting Command.
		*/
		class ConfigSetCommand : public ICommand 
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
					* \brief Enable internal data register.
					* \default 0
					*/
					bool				el:1;
			
					/**
					* \brief Frequency band selection.
					*/
					bool				ef:1;

					/**
					* \brief The frequency band.
					*/
					frequency_band_t	b:2;	

					/**
					* \brief The crystal load capacitance.
					*/
					crystal_load_t		x:4;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			ConfigSetCommand() 
				: command_word(0x8008)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }

			/**
			* \brief Sets the frequency band.
			*	
			* \param band The frequency band.
			*/
			inline void setFrequencyBand(const frequency_band_t band = FREQ_BAND_433) {
				this->b = band;
			}

			/**
			* \brief Sets the crystal load capacitance.
			*	
			* \param load The capacitance.
			*/
			inline void setCrystalCapacitance(const crystal_load_t load = CL_125) {
				this->x = load;
			}

			/**
			* \brief Sets whether the internal FIFO should be enabled.
			*	
			* \param value FIFO enabled.
			*/
			inline void enableFifo(const bool value = true) { this->ef = value; }

			/**
			* \brief Sets whether the internal data register should be enabled.
			*	
			* \param value Data register enabled.
			*/
			inline void enableData(const bool value = true) { this->el = value; }
		};
	}
}

#endif /* RFM12_CONFIGSET_H_ */