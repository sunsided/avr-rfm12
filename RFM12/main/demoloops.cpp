/*
 * demoloops.cpp
 *
 * Created: 18.06.2013 00:40:51
 *  Author: sunside
 */ 

#include "comm/usart_comm.h"
#include "main.hpp"
#include "demoloops.hpp"

/**
* \brief State of the send demo
*/
static enum {
	RXTXSTATE_IDLE,

	#if DEMO_TRANSMITTER_MODE

	RXTXSTATE_IDLE_TXOFF,			//<! System is idle and transmitter is disabled
	RXTXSTATE_IDLE_TXON,			//<! System is idle and transmitter is enabled
	RXTXSTATE_TRANSMITTING,			//<! System is transmitting

	#else
	
	RXTXSTATE_IDLE_RXOFF,			//<! System is idle and receiver is disabled
	RXTXSTATE_IDLE_RXON,			//<! System is idle and receiver is enabled
	RXTXSTATE_RECEIVING,			//<! System is receiving

	#endif
	
} rxtxdemostate = RXTXSTATE_IDLE;

#if DEMO_TRANSMITTER_MODE

/**
* \brief Starts the transmitter demo loop
*/
void transmitterDemoLoop(rfm12::Rfm12 *rfm12, ringbuffer::RingBuffer *rfm12SendBuffer, uint8_t group) 
{
	// enable fast-transmitter strategy and commit (transmitter still in idle mode)
	rfm12->setTransceiverStrategy(RXTXSTRATEGY_FAST_TRANSMITTER, true);
	usart_comm_send_zstr("transmitter configured.\r\n");
	
	// data counter
	static uint8_t counter = 0;
	
	// loopity loop.
	for(;;)
	{
		switch (rxtxdemostate)
		{
			default:
			case RXTXSTATE_IDLE_TXOFF:
			{
				// at this point, the receiver should not be active, so we may very well reset the buffer
				rfm12SendBuffer->reset();
				
				// send receiver synchronization bytes
				rfm12SendBuffer->writeSync(0xAA);
				rfm12SendBuffer->writeSync(0xAA);
				rfm12SendBuffer->writeSync(0xAA);
				rfm12SendBuffer->writeSync(0xAA);
			
				// send synchronization pattern
				rfm12SendBuffer->writeSync(0x2D);
				rfm12SendBuffer->writeSync(group);
			
				// send payload: magic pattern
				rfm12SendBuffer->writeSync(0x01);
				rfm12SendBuffer->writeSync(0x02);
				rfm12SendBuffer->writeSync(0x03);
				rfm12SendBuffer->writeSync(0x04);
				
				// send payload: user data length
				rfm12SendBuffer->writeSync(8);
				
				// send payload: user data
				rfm12SendBuffer->writeSync('H');
				rfm12SendBuffer->writeSync('O');
				rfm12SendBuffer->writeSync('L');
				rfm12SendBuffer->writeSync('Y');
				rfm12SendBuffer->writeSync('C');
				rfm12SendBuffer->writeSync('O');
				rfm12SendBuffer->writeSync('W');
				rfm12SendBuffer->writeSync(counter++);
			
				// send dummy byte
				rfm12SendBuffer->writeSync(0x00);
			
				// enable transmitter, then sleep
				rfm12->enterTransmitterMode();
				usart_comm_send_zstr("transmitter on ...\r\n");
				// sleep(1);
			
				rxtxdemostate = RXTXSTATE_TRANSMITTING;
				break;
			}
		
			case RXTXSTATE_TRANSMITTING:
			{
				// the interrupt knows best
				if (_rfm12PulseRequired) {
					rfm12->pulse();
					_rfm12PulseRequired = false;
				}
			
				// if the transmission is not done, do not switch state
				if (!rfm12->isTransmissionDone()) break;
				usart_comm_send_zstr("data sent.\r\n");
				rxtxdemostate = RXTXSTATE_IDLE_TXON;
				break;
			}
		
			case RXTXSTATE_IDLE_TXON:
			{
				// disable transmission
				rfm12->enterIdleMode();
				usart_comm_send_zstr("transmitter off.\r\n");
			
				// sleep for some time
				sleep(5);
				rxtxdemostate = RXTXSTATE_IDLE_TXOFF;
				break;
			}
		}
	}	
}

#endif  /* DEMO_TRANSMITTER_MODE */

#if DEMO_RECEIVER_MODE

/**
* \brief Starts the transmitter demo loop
*/
void receiverDemoLoop(rfm12::Rfm12 *rfm12, ringbuffer::RingBuffer *rfm12ReceiveBuffer)
{
	// enable fast-receiver strategy and commit (receiver still in idle mode)
	rfm12->setTransceiverStrategy(RXTXSTRATEGY_FAST_RECEIVER, true);
	usart_comm_send_zstr("receiver configured.\r\n");
	
	// loopity loop.
	for(;;)
	{
		switch (rxtxdemostate)
		{
			default:
			case RXTXSTATE_IDLE_RXOFF:
			{
				// at this point, the receiver should not be active, so we may very well reset the buffer
				rfm12ReceiveBuffer->reset();
			
				// enable transmitter, then sleep
				rfm12->enterReceiverMode();
				usart_comm_send_zstr("receiver on ...\r\n");
			
				rxtxdemostate = RXTXSTATE_RECEIVING;
				break;
			}
		
			case RXTXSTATE_RECEIVING:
			{
				// the interrupt knows best
				if (_rfm12PulseRequired) {
					set_led(OFF);
					rfm12->pulse();
					_rfm12PulseRequired = false;
				}
			
				// if the transmission is not done, do not switch state
				ringbuffer::rbdata_t item;
				if (rfm12ReceiveBuffer->tryRead(&item)) {
					usart_comm_send_char(item);
				}
				break;
			}
		
			case RXTXSTATE_IDLE_RXON:
			{
				// disable transmission
				rfm12->enterIdleMode();
				usart_comm_send_zstr("receiver off.\r\n");

				rxtxdemostate = RXTXSTATE_IDLE_RXOFF;
				break;
			}
		}
	}
}

#endif /* DEMO_RECEIVER_MODE */