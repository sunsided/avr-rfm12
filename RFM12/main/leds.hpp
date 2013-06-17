/*
 * leds.hpp
 *
 * Created: 18.06.2013 01:33:36
 *  Author: Markus
 */ 


#ifndef LEDS_HPP_
#define LEDS_HPP_

#include <avr/io.h>
#include "util.h"
#include "datatypes.h"

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
inline void set_led(switch_t enabled) 
{
	DDRC |= (1 << DDC2);
	
	if (enabled) {
		SET_BIT(PORTC, PORTC2);
	}
	else {
		CLEAR_BIT(PORTC, PORTC2);
	}
}

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
inline void set_red_led(switch_t enabled) 
{
	DDRC |= (1 << DDC1);
	
	if (enabled) {
		SET_BIT(PORTC, PORTC1);
	}
	else {
		CLEAR_BIT(PORTC, PORTC1);
	}
}

/*! 
	\brief Toggles the LED on and off.

*/
void led_flash_sync();

/*! 
	\brief Toggles the LED on and off.
*/
void led_doubleflash_sync();


#endif /* LEDS_HPP_ */