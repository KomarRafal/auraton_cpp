/*
 * device_id.hpp
 */

#ifndef COMMANDS_ADDRESS_HPP_
#define COMMANDS_ADDRESS_HPP_

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

namespace commands
{


class device_id : public simple_token
{
public:
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string DEVICE_ID_TOKEN = "ID: ";
};

}

}

}

#endif
