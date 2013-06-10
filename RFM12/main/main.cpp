/*
 * main.c
 *
 * Created: 02.06.2013 21:09:43
 *  Author: sunside
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

#include "system.h"

#include "comm/usart_comm.h"
#include "comm/command_decoder.h"

#include "main.h"
#include "spi/spi.h"
#include "rfm12/rfm12.h"

using namespace rfm12;

#include <util/delay.h> 

#define BIN(a,b) 0b ## a ## b

int main()
{
	#if F_CPU != 16000000UL
	#error Expected F_CPU to be 16MHz; Timer calculation will be wrong!
	#endif

	// USART
	usart_comm_init();
	usart_comm_send_zstr("SYSTEM READY\r\n");

	// Initialize status LED
	led_flash_sync();
	
	// Prepare SPI and RFM12
	spi_initialize();
	rfm12_initialize_interrupt();
	
	// Fire and go.
	sei();
		
	// initialisieren des RFM12
	_Command(0);
	_Command(0b1011001000000101); // RF_SLEEP_MODE
	_Command(0b1011100000000000); // RF_TXREG_WRITE

	while ((PIND & (1 << PIND2)) == 0)
	{
		_Command(0);
	}
	
	// signal we're ready to go
	led_doubleflash_sync();
		

	// 1000 0000 .... .... Configuration Setting Command
	uint8_t band			= 0b01; // 433 Mhz
	uint8_t capacitance		= 0b0111; // 12.0pF 
	_Command(0b1000000011000000 | (band << 4) | capacitance); // EL (ena TX), EF (ena RX FIFO), 12.0pF 
	
	// 1010 .... .... .... Frequency Setting Command
	// Bits f0..f11 m�ssen im Bereich 96 bis 3903 liegen.
	// Mittelfrequenz berechenbar gem��:
	// Fm = 10 * C1 * (C2 + F/4000) [MHz]
	double c1 =  1; // f�r 433 MHz
	double c2 = 43; // f�r 433 MHz
	double frequency = 433.0D; // MHz
	uint16_t freq_setting = (uint16_t)(10.0D * c1 * (c2 + frequency/4000));
	_Command(0b1010000000000000 | (freq_setting & 0b0000111111111111));
		
	// 1100 0110 .... .... Data Rate Command
	_Command(0b1100011000000110); // approx 49.2 Kbps, i.e. 10000/29/(1+6) Kbps
		
	// 1001 0... .... .... Receiver Control Command
	_Command(0b1001010010100010); // VDI,FAST,134kHz,0dBm gain,-91dBm RSSI detector
	// VDI = Valid Data Indicator
	
	// 1100 0010 .... .... Data Filter Command
	_Command(0b1100001010101100); // AL,!ml,DIG,DQD4
	// al = 1 -- Clock recovery auto lock control: auto mode
	// ml = 0 -- Clock recovery lock control: slow mode, slow attack, slow release
	//  s = 1 -- Digital Filter
	// DQD threshold = 4
	
	uint8_t group = 212; // 212 ist einzige f�r RFM12 -- sind zwar RFM12B, aber schaden kann es ja nicht
	if (group != 0) {
		// 1100 1010 .... .... FIFO and Reset Mode Command
		_Command(0b1100101010000011); // FIFO8,2-SYNC,!ff,DR
		
		// 1100 1110 .... .... Synchron Pattern Command
		_Command(0b1100001100000000 | group); // SYNC=2DXX?
	} else {
		// 1100 1010 .... .... FIFO and Reset Mode Command
		_Command(0b1100101010001000); // FIFO8,1-SYNC,!ff,DR
		
		// 1100 1110 .... .... Synchron Pattern Command
		_Command(0b1100111000101101); // SYNC=2D?
	}
	
	// 1100 0100 .... .... AFC Command
	_Command(0b1100010010000011); // @PWR,NO RSTRIC,!st,!fi,OE,EN
	
	// 1001 100. .... .... TX Configuration Command
	_Command(0b1001100001010000); // !mp,90kHz,MAX OUT
	
	// 1100 1100 0.... .... PLL Setting Command
	_Command(0b1100110001110111); // OB1?OB0, LPX,?ddy?DDIT?BW0
	
	// 111. .... .... ....Wake-Up Timer Command
	_Command(0b1110000000000000); // NOT USE
	
	// 1100 1000 .... .... Low Duty-Cycle Command
	_Command(0b1100100000000000); // NOT USE
	
	// 1100 0000 .... .... Low Battery Detector and Microcontroller Clock Divider Command
	_Command(0b1100000001001001); // 1.66MHz,3.1V
/**/		

	// bye.
	while(1) 
	{
		// sendet jeweils ein Byte -- multiple byte write unterst�tzt!
		/*
		// synchronize sender and receiver
		_Command(0b1011100000000000 | (0xAA & 0b0000000011111111));
		_Command(0b1011100000000000 | (0xAA & 0b0000000011111111));
		_Command(0b1011100000000000 | (0xAA & 0b0000000011111111));
		_Command(0b1011100000000000 | (0xAA & 0b0000000011111111));
		
		// synchron pattern
		_Command(0b1011100000000000 | (0x2D & 0b0000000011111111));
		_Command(0b1011100000000000 | (0xD4 & 0b0000000011111111));
		
		// payload
		uint8_t data = 227;
		_Command(0b1011100000000000 | (data & 0b0000000011111111));
		_Command(0b1011100000000000 | (data & 0b0000000011111111));
		_Command(0b1011100000000000 | (data & 0b0000000011111111));
		_Command(0b1011100000000000 | (data & 0b0000000011111111));
		
		// synchronize sender and receiver
		_Command(0b1011100000000000 | (0xAA & 0b0000000011111111));
		_Command(0b1011100000000000 | (0xAA & 0b0000000011111111));
		
		_delay_ms(500);
		*/
		// Register lesen
		// uint16_t values = _Command(0b1011000000000000);
		
		uint_least16_t status = rfm12_read_status();
		usart_comm_send_char(status >> 8);
		usart_comm_send_char(status & 0xFF);
		usart_comm_send_zstr("\r\n");
		/*
		
		_configset_Command_t cmd;
		usart_comm_send_char(sizeof(_configset_Command_t));
		usart_comm_send_char(cmd.command_word >> 8);
		usart_comm_send_char(cmd.command_word & 0xFF);
		usart_comm_send_char(cmd.x);

		usart_comm_send_char(cmd.el);
		usart_comm_send_char(cmd.ef);
		cmd.enable_data();
		usart_comm_send_char(cmd.el);
		usart_comm_send_char(cmd.ef);
		cmd.enable_fifo();
		usart_comm_send_char(cmd.el);
		usart_comm_send_char(cmd.ef);
		usart_comm_send_zstr("\r\n");
		*/
	}
}

/*! 
	\brief Enables or disables the LED
	
	\param enabled Determines if the LED should be enabled (\c ON) or disabled (\c OFF)
*/
inline void set_led(switch_t enabled) 
{
	DDRC |= (1 << DDC2);
	
	if (enabled)
		SET_BIT(PORTC, PORTC2);
	else 
		CLEAR_BIT(PORTC, PORTC2);
}


/*! 
	\brief Toggles the LED on and off.
	
	\param ms HIGH duration in milliseconds.
*/
void led_flash_sync()
{
	set_led(ON);
	_delay_ms(100);
	set_led(OFF);
}

/*! 
	\brief Toggles the LED on and off.
*/
void led_doubleflash_sync()
{
	set_led(ON);
	_delay_ms(100);
	set_led(OFF);
	_delay_ms(50);
	set_led(ON);
	_delay_ms(100);
	set_led(OFF);
}