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
		public:
			/**
			* \brief The result data of a single command call
			*/
			union
			{
				/**
				* \brief The result word.
				*/
				const uint16_t result_word;
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
			StatusCommandResult(const uint16_t data = 0) : result_word(data) {}
				
			/**
			* \brief Initializes this copy from another instance.
			*
			* \param other The instance to copy
			*/
			StatusCommandResult(const StatusCommandResult& other) : result_word(other.result_word) {}
					
			/**
			* \brief Gets the result word.
			*/
			inline uint16_t getResultWord() const { return this->result_word; }
				
			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->result_word; }
		};		
	}
}



#endif /* ISTATUSCOMMANDRESULT_H_ */