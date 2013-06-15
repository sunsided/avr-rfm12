/*
 * Command.cpp
 *
 * Created: 15.06.2013 17:45:36
 *  Author: Markus
 */ 

#include "Command.hpp"
#include "CommandResult.hpp"
#include "../Rfm12.hpp"

using namespace rfm12;
using namespace rfm12::commands;

/**
* \brief Executes the command.
*
* \returns The result
*/
const CommandResult* Command::execute()
{
	return this->_owner->executeCommand(this);
}