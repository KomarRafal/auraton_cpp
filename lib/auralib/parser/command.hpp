/*
 * command.hpp
 */

#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

class command : public simple_token {
public:
	virtual const std::string& get_token() const;

private:
	const std::string COMMAND_TOKEN = "COMMAND:";
};

}

}

#endif
