/*
 * direction.cpp
 */
#include "direction.hpp"

namespace aura
{

namespace parser
{

namespace events
{

namespace direction
{

const std::string& direction::get_token() const
{
	return DIRECTION_TOKEN;
}

bool receive::is_value_ok(const std::string_view& value) const
{
	return RECEIVE_TOKEN == value;
}

bool transmit::is_value_ok(const std::string_view& value) const
{
	return TRANSMIT_TOKEN == value;
}

}

}

}

}
