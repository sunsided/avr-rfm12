/*
 * rfm12configuration.hpp
 *
 * Created: 14.06.2013 20:02:22
 *  Author: Markus
 */ 


#ifndef RFM12CONFIGURATION_H_
#define RFM12CONFIGURATION_H_

#include <avr/io.h>
#include "rfm12/Rfm12.hpp"

#define RFM12_IRQ_PIN_DDR			DDRD		//<! Data Direction Register
#define RFM12_IRQ_PIN_DDR_BIT		DDD2		//<! Data Direction Register bit
#define RFM12_IRQ_PIN_REG			PIND		//<! Input register
#define RFM12_IRQ_PIN_REG_BIT		PIND2		//<! Input register bit

#define RFM12_IRQ_PCICR				PCICR		//<! Pin Change Interrupt Control Register
#define RFM12_IRQ_PCICR_IEBIT		PCIE2		//<! Pin Change Interrupt Control Register enable bit
#define RFM12_IRQ_PCIMASK			PCMSK2		//<! Pin Change Mask Register
#define RFM12_IRQ_PCIMASK_BIT		PCINT18		//<! Pin Change Mask Register pin bit

/**
* \brief Helper macro to read the current value of the RFM12 IRQ lane
*/
#define RFM12_INTERRUPT_PIN_VALUE (RFM12_IRQ_PIN_REG & (1 << RFM12_IRQ_PIN_REG_BIT))

/**
 * \brief Initializes the RFM12 interrupt pin.
 * 
 * \return void
 */
void initializeRfm12Interrupt();

/**
* \brief Configures the given RFM12 instance
*/
void configureRfm12(rfm12::Rfm12 *rfm12);

#endif /* RFM12CONFIGURATION_H_ */