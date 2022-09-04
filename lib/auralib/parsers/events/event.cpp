/*
 * event.cpp
 */

#include "event.hpp"

namespace aura
{

namespace parser
{

const std::string event::SOURCE_TOKEN = "EVENT";

const std::string& event::get_token() const
{
	return EVENT_TOKEN;
}

}

}
