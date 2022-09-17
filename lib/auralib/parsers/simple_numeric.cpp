/*
 * simple_numeric.cpp
 */
#include <cstdlib>

#include "simple_numeric.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

simple_numeric::simple_numeric(const std::string& specific_token) :
	token(specific_token) {
}

const std::string& simple_numeric::get_token() const
{
	return token;
}

bool simple_numeric::is_value_ok(const std::string_view& value) const
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
