/*
 * Spi.hpp
 *
 * Created: 11.06.2013 02:07:56
 *  Author: Markus
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

namespace spi
{
	/**
	* \brief SPI controller.
	*/
	class SpiMaster
	{
	private:
		// http://www.nongnu.org/avr-libc/user-manual/FAQ.html#faq_port_pass
		volatile uint8_t *_ddr_register;
		volatile uint8_t *_output_register;
		volatile uint8_t *_input_register;
		const uint8_t _mosi_bit;
		const uint8_t _miso_bit;
		const uint8_t _sck_bit;
		const uint8_t _ss_bit;
	
	public:
		/**
		* \brief Initializes the SPI controller.
		*
		* \param ddr_register		Pointer to the DDR register (MOSI, MISO, SCK, SS).
		* \param output_register	Pointer to the output register (MOSI, SCK, SS)
		* \param input_register		Pointer to the input register (MISO, SS pull up).
		* \param mosi_bit			Index of the bit that identifies the MOSI port.
		* \param miso_bit			Index of the bit that identifies the MISO port.
		* \param sck_bit			Index of the bit that identifies the SCK/CLK port.
		* \param ss_bit				Index of the bit that identifies the SS port.
		*/
		inline SpiMaster( 
					volatile uint8_t *ddr_register, 
					volatile uint8_t *output_register, 
					volatile uint8_t *input_register,
					const uint8_t mosi_bit, 
					const uint8_t miso_bit, 
					const uint8_t sck_bit, 
					const uint8_t ss_bit)
					:	_ddr_register(ddr_register),
						_output_register(output_register),
						_input_register(input_register),
						_mosi_bit(mosi_bit),
						_miso_bit(miso_bit),
						_sck_bit(sck_bit),
						_ss_bit(ss_bit)
					{}
		inline ~SpiMaster(){}
		
		/**
		* \brief Initializes the SPI.
		*/
		void initialize() const;
		
		/**
		* \brief Begins a transmission by pulling Slave Select low.
		*/
		void beginTransmission() const;
		
		/**
		* \brief Ends a transmission by pulling Slave Select high.
		*/
		void endTransmission() const;

		/**
		* \brief Transmits a byte of data.
		*
		* This method sends data synchronously by blocking on the
		* SPI send register.
		*
		* \param data The data to send.
		* \returns The result.
		*/
		uint_fast8_t transmitSync(uint_fast8_t data) const;

		/**
		* \brief Transmits two bytes of data.
		*
		* This method sends data synchronously by blocking on the
		* SPI send register.
		*
		* \param data The data to send.
		* \returns The result.
		*/
		uint_fast16_t transmitSync(uint_fast16_t data) const;
	};

}

#endif /* SPI_H_ */