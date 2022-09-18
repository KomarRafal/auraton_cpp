/*
 * specific_command.hpp
 */

#ifndef SPECIFIC_COMMAND_HPP_
#define SPECIFIC_COMMAND_HPP_

#include <string>
#include "command.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

class specific_command : public command {
public:
	explicit specific_command(const std::string& specific_command);
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string command;
};

}

}

}

#endif
