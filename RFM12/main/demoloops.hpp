/*
 * demoloops.hpp
 *
 * Created: 18.06.2013 00:41:28
 *  Author: sunside
 */ 

#ifndef __DEMOLOOPS_HPP_
#define __DEMOLOOPS_HPP_

#include <rfm12/Rfm12.hpp>
#include <ringbuffer/RingBuffer.hpp>

#define DEMO_TRANSMITTER_MODE 	true						//<! Enables the transmitter demo
#define DEMO_RECEIVER_MODE 		!DEMO_TRANSMITTER_MODE		//<! Enables the receiver demo if transmitter demo is disabled

/**
* \brief Determines if a pulse of the rfm12 instance is needed.
*
* This flag is driven by the pin change interrupt handler that processes
* the RFM12's interrupt line.
*/
extern volatile bool _rfm12PulseRequired;

/**
* \brief Starts the transmitter demo loop
*/
void transmitterDemoLoop(rfm12::Rfm12 *rfm12, ringbuffer::RingBuffer *rfm12SendBuffer, uint8_t group);

/**
* \brief Starts the transmitter demo loop
*/
void receiverDemoLoop(rfm12::Rfm12 *rfm12, ringbuffer::RingBuffer *rfm12ReceiveBuffer);

#endif
