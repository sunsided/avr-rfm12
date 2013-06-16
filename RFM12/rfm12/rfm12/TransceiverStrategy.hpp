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
	* \brief Do not apply any strategy
	*
	* This strategy will not interfere with any user settings.
	*/
	RXTXSTRATEGY_IGNORE = 0,
	
	/**
	* \brief Assume fast RX/TX switching.
	*
	* This strategy will try to leave the synthesizer, crystal oscillator 
	* and receiver baseband block enabled when switching RX and TX to
	* minimize startup time.
	*/
	RXTXSTRATEGY_FAST_SWITCHING = 1
	
} transceiverstrategy_t;



#endif /* TRANSCEIVERSTRATEGY_H_ */