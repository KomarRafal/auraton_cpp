/*
 * at.hpp
 */

#ifndef AT_HPP_
#define AT_HPP_

#include <string>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

enum at_type {
	AT_OK,
};

class at : public parser_if<at_type> {
public:
	virtual std::optional<at_type> parse(std::string_view& message);

private:
	const std::string BEGIN_TOKEN = "AT:START" + EOL;
	const std::string END_TOKEN = "AT:STOP" + EOL;
};


}

}

#endif
