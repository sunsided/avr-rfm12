/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_LOWDUTYCYCLE_H_
#define RFM12_LOWDUTYCYCLE_H_

#include <stdint.h>

/**
* \brief Low Duty-Cycle Command
*/
typedef class _rfm12_lowdutycycle_command_t {
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
			* \brief Low duty-cycle percentage control.
			*
			* This value D controls the length of the low duty-cycle during which
			* no wake-up timer interrupts are generated.
			*
			* duty-cycle = (D · 2 +1) / M · 100%
			*
			* This value is also indirectly controlled by Wake-Up Timer setting M.
			* \see rfm12_wakeuptimer_command_t
			*/
			uint8_t				d:7;
			
			/**
			* \brief Enables the low duty-cycle command
			*/
			bool				en:1;
		};
	};
	
	public:

	/**
	* \brief Initializes this instance to default values (POR)
	*/
	_rfm12_lowdutycycle_command_t() 
		: command_word(0xC80E)
	{}

	/**
	* \brief Cast operator
	*/
	inline operator uint16_t() const { return this->command_word; }

	/**
	* \brief Sets the Low Duty-Cycle mode.
	*
	* \param enabled true if the Low Duty-Cycle mode should be enabled, false otherwise.
	*/
	inline void set_enabled(const bool enabled = false) { this->en = enabled; }

} rfm12_lowdutycycle_command_t;

#endif /* RFM12_LOWDUTYCYCLE_H_ */