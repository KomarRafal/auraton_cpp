/*
 * address.cpp
 */

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
	// TODO: maybe some better checking
	if (!value.empty() && (value.size() == ADDRESS_SIZE))
	{
		return true;
	}
	return false;
}

}

}

}
