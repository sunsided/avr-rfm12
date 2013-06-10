/*
 * IRfm12CommandResult.h
 *
 * Created: 11.06.2013 00:24:58
 *  Author: Markus
 */ 

#ifndef IRFM12COMMANDRESULT_H_
#define IRFM12COMMANDRESULT_H_

namespace rfm12
{
	namespace commands
	{
		/**
		* \brief Result of an RFM12 command
		*/
		class Rfm12CommandResult
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
			Rfm12CommandResult() : _data(0) {}
				
			/**
			* \brief Initializes this copy from another instance.
			*
			* \param other The instance to copy
			*/
			Rfm12CommandResult(const Rfm12CommandResult& other) : _data(other._data) {}
		};
	}
}

#endif /* IRFM12COMMANDRESULT_H_ */