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

/**
* \brief Enters an infinite loop and blinks the signal led
*
* \param blinkCount The number of flashes per loop
*/
static void abort_blinkloop(const uint8_t blinkCount) 
{
	for(;;) 
	{
		for(uint8_t assert_i=0; assert_i<blinkCount; ++assert_i) {
			set_red_led(ON);
			_delay_ms(200);
			set_red_led(OFF);
			_delay_ms(200);
		}
		_delay_ms(800);
	}
}

#ifdef DEBUG
#define LED_ASSERT(assertion, blink) \
	if (!(assertion)) abort_blinkloop(blink);
#else
#define LED_ASSERT(assertion, blink) {}
#endif

#endif /* MAIN_H_ */