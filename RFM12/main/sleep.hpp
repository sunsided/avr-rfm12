/*
 * sleep.hpp
 *
 * Created: 18.06.2013 02:13:50
 *  Author: sunside
 */ 


#ifndef SLEEP_H_
#define SLEEP_H_

#include "system.h"
#include <avr/delay.h>
#include "leds.hpp"

/**
* \brief Sleeps for the given number of seconds
*/
static inline void sleep(uint8_t seconds) {
	for (uint8_t s = 0; s < seconds; ++s) {
		for (uint8_t loops = 0; loops < 20; ++loops) {
			_delay_ms(50);
		}
	}
}

/**
* \brief Sleeps for the given number of seconds
*/
static inline void red_sleep(uint8_t seconds) {
	set_red_led(ON);
	sleep(seconds);
	set_red_led(OFF);
}



#endif /* SLEEP_H_ */