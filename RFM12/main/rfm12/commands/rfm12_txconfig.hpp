/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_TXCONFIG_H_
#define RFM12_TXCONFIG_H_

#include <stdint.h>

/**
* \brief Relative Output Power
*/
typedef enum
{
	OUTPOW_FULL			= 0b000,		//!< 0 dB
	OUTPOW_MINUS_025	= 0b001,		//!< -2.5 dB
	OUTPOW_MINUS_050	= 0b010,		//!< -5.0 dB
	OUTPOW_MINUS_075	= 0b011,		//!< -7.5 dB
	OUTPOW_MINUS_100	= 0b100,		//!< -10 dB
	OUTPOW_MINUS_125	= 0b101,		//!< -12.5 dB
	OUTPOW_MINUS_150	= 0b110,		//!< -15 dB
	OUTPOW_MINUS_175	= 0b111,		//!< -17.5 dB
} output_power_t;

/**
* \brief Frequency Shift Keying, absolute output frequency delta f_fsk
*/
typedef enum
{
	FSKDF_15KHZ			= 0b0000,		//!< f0 +/- 15 kHz
	FSKDF_30KHZ			= 0b0001,		//!< f0 +/- 30 kHz
	FSKDF_45KHZ			= 0b0010,		//!< f0 +/- 45 kHz
	FSKDF_60KHZ			= 0b0011,		//!< f0 +/- 60 kHz
	FSKDF_75KHZ			= 0b0100,		//!< f0 +/- 75 kHz
	FSKDF_90KHZ			= 0b0101,		//!< f0 +/- 90 kHz
	FSKDF_105KHZ		= 0b0110,		//!< f0 +/- 105 kHz
	FSKDF_120KHZ		= 0b0111,		//!< f0 +/- 120 kHz
	
	FSKDF_135KHZ		= 0b1000,		//!< f0 +/- 135 kHz
	FSKDF_150KHZ		= 0b1001,		//!< f0 +/- 150 kHz
	FSKDF_165KHZ		= 0b1010,		//!< f0 +/- 165 kHz
	FSKDF_180KHZ		= 0b1011,		//!< f0 +/- 180 kHz
	FSKDF_195KHZ		= 0b1100,		//!< f0 +/- 195 kHz
	FSKDF_210KHZ		= 0b1101,		//!< f0 +/- 210 kHz
	FSKDF_225KHZ		= 0b1110,		//!< f0 +/- 225 kHz
	FSKDF_240KHZ		= 0b1111,		//!< f0 +/- 240 kHz
} fsk_deltaffsk_t;

/**
* \brief Frequency Shift Keying, frequency inversion control
*/
typedef enum
{
	FSKMODE_NORMAL		= 0b0,			//!< uninverted (logic HIGH at high frequency, logic LOW at low frequency)
	FSKMODE_INVERTED	= 0b1			//!< inverted (logic HIGH at low frequency, logic LOW at high frequency)
	
} fsk_invert_t;

/**
* \brief TX Configuration Control Command
*/
typedef class _rfm12_txconfig_command_t {
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
			const uint8_t		command_code:7;		

			/**
			* \brief FSK (Frequency Shift Keying) Modulation: Invert frequencies
			*
			* This parameter controls whether logic HIGH will be transmitted at a frequency
			* higher than or lower than the channel center frequency f0.
			*
			* If mp is FSKMODE_NORMAL, a logic HIGH will be modulated to a frequency above f0.
			* If mp is FSKMODE_INVERTED, a logic HIGH will be modulated to a frequency below f0.
			*/
			fsk_invert_t		mp:1;
			
			/**
			* \brief FSK (Frequency Shift Keying) Modulation: Delta f_fsk
			*
			* This value controls the frequency deviation from the channel center frequency
			* at which data bits will be modulated. At FSKDF_15KHZ, data bits will be modulated
			* to channel center frequency f0 +/- 15 kHz, depending on the sign of the data bit
			* and the inversion mode.
			*/
			fsk_deltaffsk_t		m:4;
			
			
			/**
			* \brief Padding bit
			*/
			const uint8_t		:1;
			
			/**
			* \brief Relative antenna output power.
			
			* The relative output in multiples of -2.5 dB, where 0b000 means full power (-0 dB).
			*/
			output_power_t		p:3;
		};
	};
	
	public:

	/**
	* \brief Initializes this instance to default values (POR)
	*/
	_rfm12_txconfig_command_t() 
		: command_word(0x9800)
	{}

	/**
	* \brief Cast operator
	*/
	inline operator uint16_t() const { return this->command_word; }

	/**
	* \brief Sets the FSK parameters
	*
	* \param frequency The delta frequency
	* \param inversion The inversion mode
	*/
	inline void set_fsk(const fsk_deltaffsk_t frequency = FSKDF_15KHZ, const fsk_invert_t inversion = FSKMODE_NORMAL) {
		this->m = frequency;
		this->mp = inversion;
	}

} Rfm12TxConfigCommand;

#endif /* RFM12_TXCONFIG_H_ */