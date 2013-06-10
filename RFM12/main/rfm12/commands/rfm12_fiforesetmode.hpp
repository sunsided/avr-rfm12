/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_FIFORESETMODE_H_
#define RFM12_FIFORESETMODE_H_

#include <stdint.h>
#include "../IRfm12Command.h"

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Synchron pattern mode
		*/
		typedef enum
		{
			SP_TWO_BYTE = 0b0,		//<! Use a two-byte synchron pattern (2Dxx)
			SP_ONE_BYTE = 0b1,		//<! Use a one-byte synchron pattern (xx)
		} synchron_pattern_mode_t;

		/**
		* \brief FIFO Start Condition
		*/
		typedef enum
		{
			FIFOSTART_SYNCHRON		= 0b0,		//<! Start on synchron pattern match
			FIFOSTART_ALWAYSFILL	= 0b1,		//<! Always fill the FIFO
		} fifo_start_t;

		/**
		* \brief Sensitive Reset Mode
		*/
		typedef enum
		{
			RESETMOPDE_SENSITIVE	= 0b0,		//<! Sensitive Reset
			RESETMODE_NONSENSITIVE	= 0b1,		//<! Non-Sensitive Reset
		} sensitive_reset_t;

		/**
		* \brief FIFO and Reset Mode Command
		*/
		class Rfm12FifoAndResetModeCommand : public IRfm12Command {
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {
					/**
					* \brief The command code.
					*/
					const uint8_t			command_code:8;		

					/**
					* \brief FIFO fill level required for interrupt.
					*/
					uint8_t					f:4;
			
					/**
					* \brief Synchron pattern mode
					*/
					synchron_pattern_mode_t sp:1;
			
					/**
					* \brief FIFO fill start condition
					*/
					fifo_start_t			al:1;
			
					/**
					* \brief Enable FIFO fill after synchron pattern reception.
					*
					* FIFO fill stops when this bit is cleared.
					* To restart the synchron pattern recognition, this value should be toggled (i.e. cleared, then set).
					*/
					bool					ff:1;
			
					/**
					* \brief Highly Sensitive Reset mode
					*/
					sensitive_reset_t		dr:1;
				};
			};
	
			public:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			Rfm12FifoAndResetModeCommand() 
				: command_word(0xCA80)
			{}

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
		};
	}
}

#endif /* RFM12_FIFORESETMODE_H_ */