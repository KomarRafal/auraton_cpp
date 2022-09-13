/*
 * device_id.cpp
 */
#include <cstdlib>

#include "device_id.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

const std::string& device_id::get_token() const
{
	return DEVICE_ID_TOKEN;
}

bool device_id::is_value_ok(const std::string_view& value) const
{
	if (value.empty()) {
		return false;
	}
	char *end;
	const char *start = (static_cast<std::string>(value)).c_str();
	if (std::strtoul(start, &end, 10) == 0) 	{
		return false;
	}
	return true;
}

}

}

}
