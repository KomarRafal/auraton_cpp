/*
 * address.cpp
 */
#include <cstdlib>

#include "address.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

const std::string& address::get_token() const
{
	return ADDRESS_TOKEN;
}

bool address::is_value_ok(const std::string_view& value) const
{
	if (value.empty() || (value.size() != ADDRESS_SIZE)) {
		return false;
	}
	char *end;
	const char *start = (static_cast<std::string>(value)).c_str();
	if (std::strtoul(start, &end, 16) == 0) 	{
		return false;
	}
	if (end != (start + ADDRESS_SIZE))
	{
		return false;
	}
	return true;
}

}

}

}
