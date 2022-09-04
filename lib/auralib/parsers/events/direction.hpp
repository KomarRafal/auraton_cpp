/*
 * direction.hpp
 */

#ifndef DIRECTION_HPP
#define DIRECTION_HPP

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

namespace events
{

namespace direction
{

class direction : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string DIRECTION_TOKEN = "DIRECTION:";
};

class receive : public direction
{
public:
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string RECEIVE_TOKEN = "RECEIVE";
};

class transmit : public direction
{
public:
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string TRANSMIT_TOKEN = "TRANSMIT";
};

}

}

}

}
#endif
