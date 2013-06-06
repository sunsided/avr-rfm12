/*
 * rfm12.h
 *
 * Created: 02.06.2013 21:40:57
 *  Author: sunside
 */ 


#ifndef RFM12_H_
#define RFM12_H_

#include <stdint.h>
#include "commands/rfm12_configset.hpp"
#include "commands/rfm12_powermgmt.hpp"
#include "commands/rfm12_frequency.hpp"
#include "commands/rfm12_datarate.hpp"
#include "commands/rfm12_receivercontrol.hpp"
#include "commands/rfm12_datafilter.hpp"
#include "commands/rfm12_fiforesetmode.hpp"
#include "commands/rfm12_synchronpattern.hpp"
#include "commands/rfm12_fiforead.hpp"
#include "commands/rfm12_txwrite.hpp"
#include "commands/rfm12_pllsetting.hpp"
#include "commands/rfm12_txwrite.hpp"
#include "commands/rfm12_wakeuptimer.hpp"
#include "commands/rfm12_lowdutycycle.hpp"
#include "commands/rfm12_batteryclockdivider.hpp"
#include "commands/rfm12_statusread.hpp"

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