/*
 * IStatusCommandResult.h
 *
 * Created: 11.06.2013 00:43:26
 *  Author: Markus
 */ 


#ifndef ISTATUSCOMMANDRESULT_H_
#define ISTATUSCOMMANDRESULT_H_

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Result of an RFM12 command
		*/
		class StatusCommandResult
		{
			private:
			/**
			* \brief The result data of a single command call
			*/
			union
			{
				/**
				* \brief The result word.
				*/
				uint16_t result_word;
				struct {
					/**
					* \brief AFC offset value (to be added to the value of the frequency control parameter)
					*/
					const uint8_t offs:4;
					
					/**
					* \brief AFC offset sign bit (MSB)
					*/
					const uint8_t offs_sign:1;
					
					/**
					* \brief Toggling in each AFC cycle
					*/
					const uint8_t atgl:1;
					
					/**
					* \brief Clock recovery locked
					*/
					const uint8_t crl:1;
					
					/**
					* \brief Data Quality Detector output
					*/
					const uint8_t dqd:1;
					
					/**
					* \brief RSSI (when in receive mode) / ATS (when in transmit mode)
					*/
					const uint8_t rssi_ats:1;
					
					/**
					* \brief FIFO is empty
					*/
					const uint8_t ffem:1;
					
					/**
					* \brief Low battery detect
					*/
					const uint8_t lbd:1;
					
					/**
					* \brief Logic level on interrupt pin (16) changed to low
					*/
					const uint8_t ext:1;
					
					/**
					* \brief Wake-up timer overflow
					*/
					const uint8_t wkup:1;
					
					/**
					* \brief FFOV (when in receive mode) / RGUR (when in transmit mode)
					*/
					const uint8_t ffov_rgur:1;
					
					/**
					* \brief Power-on Reset
					*/
					const uint8_t por:1;
					
					/**
					* \brief RGIT (when in transmit mode) / FFIT (when in receive mode)
					*/
					const uint8_t rgit_ffit:1;
				};
			};
		public:
			/**
			* \brief Initializes this instance as empty.
			*
			* \param data The data.
			*/
			StatusCommandResult() : result_word(0) {}
		
			/**
			* \brief Initializes this instance as empty.
			*
			* \param data The data.
			*/
			StatusCommandResult(const uint16_t data) : result_word(data) {}
				
			/**
			* \brief Initializes this copy from another instance.
			*
			* \param other The instance to copy
			*/
			StatusCommandResult(const StatusCommandResult& other) : result_word(other.result_word) {}
					
			/**
			* \brief Applies a result to the status word
			*
			* \param result The value to set.
			*/
			inline void applyResult(const uint16_t result) { this->result_word = result; }
					
			/**
			* \brief Gets the result word.
			*/
			inline uint16_t getResultWord() const { return this->result_word; }
				
			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->result_word; }
				
			/**
			* \brief TX register is ready to receive the next byte 
			*
			* Can be cleared by Transmitter Register Write Command.
			*
			* \returns true if the transmit register is ready to receive a byte.
			*/
			inline const bool isTransmitRegisterReady() const { return rgit_ffit; }
				
			/**
			* \brief The number of data bits in the RX FIFO has reached the pre-programmed limit 
			*
			* Can be cleared by any of the FIFO read methods.
			*
			* \returns
			*/
			inline const bool isFifoLevelReached() const { return rgit_ffit; }
				
			/**
			* \brief Power-on reset
			*
			* Cleared after Status Read Command
			*
			* \returns true if we are/were in power-on reset
			*/
			inline const bool isPowerOnReset() const { return por; }
				
			/**
			* \brief TX register under run, register over write
			*
			* Cleared after Status Read Command.
			*
			* \returns true if the transmit buffer is underrun
			*/
			inline const bool isTransmitRegisterUnderrun() const { return ffov_rgur; }
				
			/**
			* \brief RX FIFO overflow
			*
			* Cleared after Status Read Command.
			*
			* \returns true if the receive FIFO is overflown
			*/
			inline const bool isReceiverFifoOverflown() const { return ffov_rgur; }
				
			/**
			* \brief Wake-up timer overflow
			*
			* Cleared after Status Read Command.
			*
			* \returns true if a wake-up was triggered
			*/
			inline const bool isWakeUpTimerOverflown() const { return wkup; }
				
			/**
			* \brief Logic level on interrupt pin (pin 16) changed to low
			*
			* Cleared after Status Read Command.
			*
			* \returns true if an external interrupt occured
			*/
			inline const bool isExternalInterruptLow() const { return ext; }
				
			/**
			* \brief Low battery detect, the power supply voltage is below the pre-programmed limit
			*
			* \returns true if the battery voltage level is low
			*/
			inline const bool isLowBatteryDetected() const { return lbd; }
				
			/**
			* \brief FIFO is empty
			*
			* \returns true if the receiver FIFO is empty
			*/
			inline const bool isReceiverFifoEmpty() const { return ffem; }
				
			/**
			* \brief Antenna tuning circuit detected strong enough RF signal
			*
			* \returns true if a strong signal is being received
			*/
			inline const bool isStrongRFSignal() const { return rssi_ats; }
				
			/**
			* \brief The strength of the incoming signal is above the pre-programmed limit
			*
			* \returns true if RSSI is high
			*/
			inline const bool isHighRSSI() const { return rssi_ats; }
				
			/**
			* \brief Data quality detector output
			*
			* \returns true if high signal quality is detected
			*/
			inline const bool isDataQualityDetected() const { return dqd; }
				
			/**
			* \brief Clock recovery locked
			*
			* \returns true if recovery is locked
			*/
			inline const bool isClockRecoveryLocked() const { return crl; }
				
			/**
			* \brief Toggling in each AFC cycle
			*
			* \returns Binary value.
			*/
			inline const bool getAFCCycleIndicator() const { return atgl; }
				
			/**
			* \brief Offset value to be added to the value of the frequency control parameter
			*
			* \returns The frequency offset
			*/
			inline const uint8_t getFrequencyOffset() const { return (offs_sign ? 0b11110000 : 0b0000000) | offs; }
		};		
	}
}



#endif /* ISTATUSCOMMANDRESULT_H_ */