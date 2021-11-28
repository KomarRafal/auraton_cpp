/*
 * address.hpp
 */

#ifndef COMMANDS_ADDRESS_HPP_
#define COMMANDS_ADDRESS_HPP_

#include "simple_value.hpp"

namespace aura
{

namespace parser
{

namespace commands
{


class address : public simple_value
{
public:
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string ADDRESS_TOKEN = "ADDRESS: "; //TODO: remove additional space
	const uint8_t ADDRESS_SIZE = 8;
};

}

}

}

#endif
