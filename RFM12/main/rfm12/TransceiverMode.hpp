/*
 * TransceiverMode.hpp
 *
 * Created: 16.06.2013 01:02:25
 *  Author: Markus
 */ 


#ifndef TRANSCEIVERMODE_H_
#define TRANSCEIVERMODE_H_

/**
* \brief The transceiver mode
*/
typedef enum {
	RXTXMODE_IDLE	= 0U,	//<! Transceiver in idle mode
	RXTXMODE_RX		= 1U,	//<! Transceiver in receive mode
	RXTXMODE_TX		= 2U,	//<! Transceiver in transmit mode
} transceivermode_t;



#endif /* TRANSCEIVERMODE_H_ */