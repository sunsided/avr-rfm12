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
			* \brief
			*/
			uint8_t				mp:1;
			
			/**
			* \brief
			*/
			uint8_t				m:4;
			
			
			/**
			* \brief Padding bit
			*/
			const uint8_t		:1;
			
			/**
			* \brief 
			*/
			uint8_t				p:3;
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
	* \brief Cast-Operator
	*/
	inline operator uint16_t() const { return this->command_word; }

} rfm12_txconfig_command_t;

#else
#error Dual Include
#endif /* RFM12_TXCONFIG_H_ */