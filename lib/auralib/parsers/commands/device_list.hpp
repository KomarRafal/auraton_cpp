/*
 * device_list.hpp
 */

#ifndef COMMANDS_DEVICE_LIST_HPP_
#define COMMANDS_DEVICE_LIST_HPP_

#include <string_view>

#include "parser_if.hpp"

namespace aura
{

namespace parser
{

namespace commands
{


class device_list : public parser_if
{
public:
	virtual parsed_value parse(std::string_view& message);

private:
	const std::string DEVICE_TOKEN = "ID: ";
};

}

}

}

#endif
