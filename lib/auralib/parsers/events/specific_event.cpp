/*
 * specific_event.cpp
 */

#include "specific_event.hpp"

namespace aura
{

namespace parser
{

namespace events
{

specific_event::specific_event(const std::string& specific_event) :
	event(specific_event) {
}

bool specific_event::is_value_ok(const std::string_view& value) const {
	return value == event;
}

}

}

}
