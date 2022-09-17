/*
 * device_parameter.hpp
 */

#ifndef COMMANDS_DEVICE_PARAMETER_HPP_
#define COMMANDS_DEVICE_PARAMETER_HPP_

#include <string_view>

#include "parser_if.hpp"

namespace aura
{

namespace parser
{

namespace commands
{


class device_parameter : public parser_if
{
public:
	virtual parsed_value parse(std::string_view& message);

private:
	const std::string DEVICE_PARAMETER_TOKEN = "CODE: ";
};

}

}

}

#endif
