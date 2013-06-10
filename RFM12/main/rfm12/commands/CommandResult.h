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
	namespace commands
	{
		/**
		* \brief Result of an RFM12 command
		*/
		class CommandResult
		{
		private:
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
			* \brief Initializes this copy from another instance.
			*
			* \param other The instance to copy
			*/
			CommandResult(const CommandResult& other) : _data(other._data) {}
		};
	}
}

#endif /* ICommandRESULT_H_ */