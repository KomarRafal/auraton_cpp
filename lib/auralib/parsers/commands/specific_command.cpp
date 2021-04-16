/*
 * specific_command.cpp
 */

#include "specific_command.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

specific_command::specific_command(std::string specific_command) :
	command(specific_command) {
}

bool specific_command::is_value_ok(const std::string_view& value) const {
	return value == command;
}

}

}

}
