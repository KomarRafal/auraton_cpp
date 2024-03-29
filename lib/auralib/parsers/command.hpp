/*
 * command.hpp
 */

// TODO: there are two headers: command.hpp.
// Add prefix to include guard "PARSERS" and remove trailing "_"
#ifndef COMMAND_HPP_
#define COMMAND_HPP_

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

class command : public simple_token {
public:
	static const std::string SOURCE_TOKEN;

	virtual const std::string& get_token() const;

private:
	// TODO: remove whitespace after fw fix
	const std::string COMMAND_TOKEN = "COMMAND: ";
};

}

}

#endif
