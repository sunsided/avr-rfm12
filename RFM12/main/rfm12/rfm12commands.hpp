/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12COMMANDS_H_
#define RFM12COMMANDS_H_

typedef enum {
	FREQ_BAND_RESERVED = 0,
	FREQ_BAND_433 = 1,
	FREQ_BAND_868 = 2,
	FREQ_BAND_915 = 3,
} frequency_band_t;

typedef class _rfm12_configset_command_t {
	
	public:
	union {
		/**
		* \brief The raw command word.
		*/
		uint16_t command_word;
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
			uint8_t				x:4;
		};
	};
	
	public:
	_rfm12_configset_command_t() 
		: command_word(0x8008)
	{}
	
	} rfm12_configset_command_t;


#endif /* RFM12COMMANDS_H_ */