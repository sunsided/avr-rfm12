/*
 * rfm12commands.h
 *
 * Created: 06.06.2013 08:45:55
 *  Author: sunside
 */ 


#ifndef RFM12_FIFORESETMODE_H_
#define RFM12_FIFORESETMODE_H_

#include <stdint.h>
#include "Command.hpp"

namespace rfm12
{
	class Rfm12;
	
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
		class FifoAndResetModeCommand : public Command {
			friend class rfm12::Rfm12;
			
			public:
			union {
				/**
				* \brief The raw command word.
				*/
				const uint16_t command_word;
				struct {		
					/**
					* \brief Highly Sensitive Reset mode
					*/
					sensitive_reset_t		dr:1;
					
					/**
					* \brief Enable FIFO fill after synchron pattern reception.
					*
					* FIFO fill stops when this bit is cleared.
					* To restart the synchron pattern recognition, this value should be toggled (i.e. cleared, then set).
					*/
					bool					ff:1;
					
					/**
					* \brief FIFO fill start condition
					*/
					fifo_start_t			al:1;
					
					/**
					* \brief Synchron pattern mode
					*/
					synchron_pattern_mode_t sp:1;
					
					/**
					* \brief FIFO fill level required for interrupt.
					*/
					uint8_t					f:4;
					
					/**
					* \brief The command code.
					*/
					const uint8_t			command_code:8;		
				};
			};
	
			private:

			/**
			* \brief Initializes this instance to default values (POR)
			*/
			FifoAndResetModeCommand(Rfm12 *owner) 
				: Command(owner), command_word(0xCA80)
			{}

			public:

			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->command_word; }
				
			/**
			* \brief Sets the synchron pattern length
			*
			* \param value The pattern length
			*/
			inline void setSynchronPatternLength(const synchron_pattern_mode_t value = SP_TWO_BYTE) { this->sp = value; }
				
			/**
			* \brief Sets the FIFO IT level.
			* 
			* The FIFO generates IT when the number of received data bits reaches this level.
			*
			* \param value The fill level
			*/
			inline void setFifoInterruptFillLevel(const uint8_t value = 8) { this->f = (value < 15 ? value : 15); }
				
			/**
			* \brief Sets the FIFO fill start condition
			*
			* \param value The start condition
			*/
			inline void setFifoFillStartCondition(const fifo_start_t value = FIFOSTART_SYNCHRON) { this->al = value; }
				
			/**
			* \brief Sets whether the FIFO fill should start after the reception of the synchron pattern.
			*
			* \param enabled FIFO fill enabled if true, otherwise FIFO fill is disabled.
			*/
			inline void setFifoFillAfterSynchronMatchEnabled(const bool enabled = true) { this->ff = enabled; }
				
			/**
			* \brief Sets the highly sensitive reset mode.
			*
			* \param value The reset mode.
			*/
			inline void setSensitiveResetMode(const sensitive_reset_t value = RESETMOPDE_SENSITIVE) { this->dr = value; }
				
			/**
			* \brief Gets this command's type.
			*
			* \return The command type
			*/
			inline commandtype_t getCommandType() const {
				return RFM12CMD_FIFOANDRESETMODE;
			}
		};
	}
}

#endif /* RFM12_FIFORESETMODE_H_ */