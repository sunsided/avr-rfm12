/*
 * rfm12.h
 *
 * Created: 02.06.2013 21:40:57
 *  Author: sunside
 */ 


#ifndef RFM12_H_
#define RFM12_H_

#include <stdint.h>

/**
 * \brief Initializes the RFM12 interrupt pin.
 * 
 * \return void
 */
void rfm12_initialize_interrupt();

/**
 * \brief Sends a command to the RFM12.
 *
 * \param command The command word
 *
 * \return The result
 */
uint_least16_t rfm12_command(uint_least16_t command);

/**
 * \brief Reads the status register from the RFM12.
 *
 * \return Status word.
 */
uint_least16_t rfm12_read_status();

#endif /* RFM12_H_ */