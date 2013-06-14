/*
 * main.h
 *
 * Created: 02.06.2013 21:14:43
 *  Author: sunside
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#include "avrgcc-compat.hpp"
#include "util.h"
#include "datatypes.h"

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
void set_led(switch_t enabled);

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

#endif /* MAIN_H_ */