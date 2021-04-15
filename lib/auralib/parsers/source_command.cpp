/*
 * source_command.cpp
 */

#include "source_command.hpp"

namespace aura
{

namespace parser
{

bool source_command::is_value_ok(const std::string_view& value) const
{
	return value == "COMMAND";
}

}

}
