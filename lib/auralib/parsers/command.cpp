/*
 * command.cpp
 */

#include "command.hpp"

namespace aura
{

namespace parser
{

const std::string command::SOURCE_TOKEN = "COMMAND";

const std::string& command::get_token() const
{
	return COMMAND_TOKEN;
}

}

}
