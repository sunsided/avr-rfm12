/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_POWERMGMT_H_
#define RFM12_POWERMGMT_H_

#include <stdint.h>

/**
* \brief Power Management Command
*/
typedef class _rfm12_powermgmt_command_t {
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

		};
	};
	
	public:

	/**
	* \brief Initializes this instance to default values (POR)
	*/
	_rfm12_powermgmt_command_t() 
		: command_word(0x8208)
	{}

	/**
	* \brief Cast-Operator
	*/
	inline operator uint16_t() const { return this->command_word; }

} rfm12_powermgmt_command_t;

#else
#error Dual Include
#endif /* RFM12_POWERMGMT_H_ */