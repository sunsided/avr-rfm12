/*
 * demoloops.cpp
 *
 * Created: 18.06.2013 00:40:51
 *  Author: sunside
 */ 

#include "comm/usart_comm.h"
#include "leds.hpp"
#include "sleep.hpp"
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
	RXTXSTATE_RECEIVING,			//<! System is receiving
	RXTXSTATE_RECEIVING_MPMATCH,	//<! Matching the magic pattern
	RXTXSTATE_RECEIVING_READSIZE,	//<! Reading the size
	RXTXSTATE_RECEIVING_READPAYLOAD,//<! Reading the payload
	RXTXSTATE_RESYNC_RXON,			//<! System is receiving but needs to re-sync

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
				rfm12SendBuffer->writeSync('H'); // 0x48
				rfm12SendBuffer->writeSync('O'); // 0x4F
				rfm12SendBuffer->writeSync('L'); // 0x4C
				rfm12SendBuffer->writeSync('Y'); // 0x59
				rfm12SendBuffer->writeSync('C'); // 0x43
				rfm12SendBuffer->writeSync('O'); // 0x4F
				rfm12SendBuffer->writeSync('W'); // 0x57
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
	
	// magic pattern match
	static uint8_t magicpattern = 0;
	
	// The payload size
	static uint8_t remainingpayload = 0;
	
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
				rxtxdemostate = RXTXSTATE_RECEIVING_MPMATCH;
				// fall through ...
			}
			
			case RXTXSTATE_RECEIVING_MPMATCH:
			case RXTXSTATE_RECEIVING_READSIZE:
			case RXTXSTATE_RECEIVING_READPAYLOAD:
			{
				// the interrupt knows best
				if (_rfm12PulseRequired) {
					set_led(OFF);
					rfm12->pulse();
					_rfm12PulseRequired = false;
				}
			
				// if the transmission is not done, do not switch state
				ringbuffer::rbdata_t item;
				if (!rfm12ReceiveBuffer->tryRead(&item)) {
					break;
				}
				
				// receive-state behaviour
				switch (rxtxdemostate)
				{
					default:
					{
						// well ... that shouldn't happen.
						rxtxdemostate = RXTXSTATE_RESYNC_RXON;
						usart_comm_send_zstr("wrong state.\r\n");
						break;
					}
					
					case RXTXSTATE_RECEIVING_MPMATCH: 
					{						
						// on payload mismatch, reset.
						if (++magicpattern != item) {
							rxtxdemostate = RXTXSTATE_RESYNC_RXON;
							usart_comm_send_zstr("!mp\r\n");
							break;
						}
						
						usart_comm_send_zstr("mp:");
						usart_comm_send_char(magicpattern + '0');
						usart_comm_send_zstr("\r\n");
						
						// if magic pattern matched, switch state
						if (magicpattern == 4) {
							rxtxdemostate = RXTXSTATE_RECEIVING_READSIZE;
							usart_comm_send_zstr("magic pattern matched.\r\n");
							break;
						}
						break;	
					}
					case RXTXSTATE_RECEIVING_READSIZE:
					{
						// read the payload size, then switch state
						remainingpayload = item;
						usart_comm_send_zstr("size:");
						usart_comm_send_char(remainingpayload);
						usart_comm_send_zstr("\r\n");
						
						rxtxdemostate = RXTXSTATE_RECEIVING_READPAYLOAD;
						break;
					}
					case RXTXSTATE_RECEIVING_READPAYLOAD:
					{
						// print data until remaining payload size is zero, then switch state
						usart_comm_send_char(item);
						if (0 == --remainingpayload) {
							usart_comm_send_zstr("\r\nend of line.\r\n");
							rxtxdemostate = RXTXSTATE_RESYNC_RXON;
							break;
						}
						break;
					}
				}
				
				break;
			}
		
			case RXTXSTATE_RESYNC_RXON:
			{
				// reset payload detection
				magicpattern = 0;
				remainingpayload = 0;
				
				
				
				// restart reception
				rfm12->resyncReceiver();
				
				// at this point, the receiver is hopefully not be receiving ...
				rfm12ReceiveBuffer->reset();
				
				usart_comm_send_zstr("resync.\r\n");

				rxtxdemostate = RXTXSTATE_RECEIVING;
				break;
			}
		}
	}
}

#endif /* DEMO_RECEIVER_MODE */