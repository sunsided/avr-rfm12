/*
 * ICommandResult.h
 *
 * Created: 11.06.2013 00:24:58
 *  Author: Markus
 */ 

#ifndef ICommandRESULT_H_
#define ICommandRESULT_H_

namespace rfm12
{
	class Rfm12;
	
	namespace commands
	{
		/**
		* \brief Result of an RFM12 command
		*/
		class CommandResult
		{
			friend class rfm12::Rfm12;
			
		protected:
			/**
			* \brief The result data of a single command call
			*/
			uint16_t _data;
		public:
		
			/**
			* \brief Initializes this instance as empty.
			*/
			CommandResult() : _data(0) {}
			
			/**
			* \brief Initializes this instance as empty.
			*
			* \param data The data.
			*/
			CommandResult(const uint16_t data) : _data(data) {}
				
			/**
			* \brief Initializes this copy from another instance.
			*
			* \param other The instance to copy
			*/
			CommandResult(const CommandResult& other) : _data(other._data) {}
				
			/**
			* \brief Gets the result word.
			*/
			const inline uint_fast16_t getResultWord() const { return this->_data; }
				
			/**
			* \brief Gets the result word's lower byte.
			*/
			inline const uint_fast8_t getLowerResultByte() const { return this->_data & 0b0000000011111111; }
			
			/**
			* \brief Gets the result word's upper byte.
			*/
			inline const uint_fast8_t getUpperResultByte() const { return (this->_data >> 8); }
				
			/**
			* \brief Applies a result to the status word
			*
			* \param result The value to set.
			*/
			inline void applyResult(const uint16_t result) { this->_data = result; }
				
			/**
			* \brief Cast operator
			*/
			inline operator uint16_t() const { return this->_data; }
		};
	}
}

#endif /* ICommandRESULT_H_ */