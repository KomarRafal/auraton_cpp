/*
 * event.hpp
 */
#ifndef EVENT_HPP
#define EVENT_HPP

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

class event : public simple_token {
public:
	static const std::string SOURCE_TOKEN;

	virtual const std::string& get_token() const;

private:
	const std::string EVENT_TOKEN = "EVENT:";
};

}

}

#endif
