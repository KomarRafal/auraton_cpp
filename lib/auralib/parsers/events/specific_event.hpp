/*
 * specific_event.hpp
 */

#ifndef SPECIFIC_EVENT_HPP_
#define SPECIFIC_EVENT_HPP_

#include <string>
#include "event.hpp"

namespace aura
{

namespace parser
{

namespace events
{

class specific_event : public event {
public:
	explicit specific_event(const std::string& specific_event);
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	std::string event;
};

}

}

}

#endif
