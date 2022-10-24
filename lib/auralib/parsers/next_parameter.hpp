/*
 * next_parameter.hpp
 */

#ifndef NEXT_PARAMETER_HPP_
#define NEXT_PARAMETER_HPP_

#include <string_view>

#include "token_traverse.hpp"
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class next_parameter : public token_traverse
{
public:
	next_parameter();

private:
	static inline const std::string PARAMETER_TOKEN = "CODE: ";
};

}

}

#endif
