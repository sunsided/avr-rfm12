/*
 * rfm12.c
 *
 * Created: 02.06.2013 21:40:16
 *  Author: sunside
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "../main.h"
#include "ISpi.hpp"
#include "Rfm12.hpp"

namespace rfm12
{
	/**
	 * \brief Initializes the RFM12 interrupt pin.
	 * 
	 * \return void
	 */
	void rfm12_initialize_interrupt()
	{
		// DDRC: Data Direction Register, Port D
		DDRD &= ~(1 << DDD2);		// Pin D2 als Eingang setzen

		// PCICR: Pin Change Interrupt Control Register
		PCICR |= (1 << PCIE2);		// PCMSK2-Überprüfung aktivieren
	
		// PCMSK: Pin Change Mask Register
		PCMSK2 |= (1 << PCINT18);	// Pin D2 triggert interrupt
	}

	/**
	 * \brief Sends a command to the RFM12.
	 *
	 * \param command The command word
	 *
	 * \return The result
	 */
	inline const uint_fast16_t Rfm12::executeCommandInternal(const uint_least16_t command_code) const
	{
		_spi->beginTransmission();
		uint_fast16_t result = _spi->transmitWord(command_code);
		_spi->endTransmission();

		// uint_fast16_t result  = spi_transmit((command_code >> 8) & 0xFF) << 8;
		// 			   result |= spi_transmit(command_code & 0xFF);
		
		return result;
	}

	/**
	 * \brief Sends a command to the RFM12.
	 *
	 * \param command The command word
	 *
	 * \return The result
	 */
	const commands::CommandResult Rfm12::executeCommandRaw(const uint_least16_t command_code) const
	{
		const uint16_t result = executeCommandInternal(command_code);
		
		// wrap in beautiful paper
		commands::CommandResult commandResult(result);
		return commandResult;
	}

	/**
	 * \brief Reads the status register from the RFM12.
	 *
	 * \return Status byte
	 */
	const commands::StatusCommandResult Rfm12::readStatus() const
	{
		static commands::StatusReadCommand readCommand;
		const uint16_t result = executeCommandInternal(readCommand.getCommandWord());
		
		// wrap in beautiful paper
		commands::StatusCommandResult commandResult(result);
		return commandResult;
	}

}

/*! 
	\brief Implements the interrupt service routine for INT0 (e.g. a pin level change)
*/
ISR (PCINT2_vect)
{
	/*
	static uint_least8_t last_value = 0;
	uint_least8_t current_value = PIND & (1 << PIND2);
	
	// RFM12 INT ist active low
	if (current_value < last_value) 
	{
		set_internal_led(ON);
	}
	else if (current_value > last_value)
	{
		set_internal_led(OFF);
	}
	
	last_value = current_value;
	*/
}	