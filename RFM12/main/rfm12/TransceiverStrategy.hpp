/*
 * TransceiverStrategy.hpp
 *
 * Created: 16.06.2013 00:30:42
 *  Author: Markus
 */ 


#ifndef TRANSCEIVERSTRATEGY_H_
#define TRANSCEIVERSTRATEGY_H_

/**
* \brief The transceiver strategy
*/
typedef enum {
	/**
	* \brief Assume fast RX/TX switching.
	*
	* This strategy will try to leave the synthesizer, crystal oscillator 
	* and receiver baseband block enabled when switching RX and TX to
	* minimize startup time.
	*/
	RXTXSTRATEGY_FAST_SWITCHING = 0
	
} transceiverstrategy_t;



#endif /* TRANSCEIVERSTRATEGY_H_ */