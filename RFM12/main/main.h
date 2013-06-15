/*
 * main.h
 *
 * Created: 02.06.2013 21:14:43
 *  Author: sunside
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include <assert.h>

#include "system.h"
#include <util/delay.h>

#include "avrgcc-compat.hpp"
#include "util.h"
#include "datatypes.h"

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
void set_led(switch_t enabled);

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
void set_red_led(switch_t enabled);

/*! 
	\brief Toggles the LED on and off.

*/
void led_flash_sync();

/*! 
	\brief Toggles the LED on and off.
*/
void led_doubleflash_sync();

/**
 * \brief Initializes the RFM12 interrupt pin.
 * 
 * \return void
 */
void initialize_rfm12_interrupt_pin();

/**
* \brief Sleeps for the given number of seconds
*/
static inline void sleep(uint8_t seconds) {
	set_red_led(ON);
	for (uint8_t s = 0; s < seconds; ++s) {
		for (uint8_t loops = 0; loops < 10; ++loops) {
			_delay_ms(100);
		}
	}
	set_red_led(OFF);
}

#ifdef DEBUG
#define LED_ASSERT(assertion, blink) \
	if (!(assertion)) { \
		for(;;) { \
			for(uint8_t assert_i=0; assert_i<(blink); ++assert_i) { \
				led_flash_sync(); \
				_delay_ms(500); \
			} \
		} \
	}
#else
#define LED_ASSERT(assertion, blink) {}
#endif

#endif /* MAIN_H_ */