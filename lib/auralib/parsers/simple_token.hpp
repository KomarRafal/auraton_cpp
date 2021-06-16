/*
 * simple_token.hpp
 */

#ifndef SIMPLE_TOKEN_HPP_
#define SIMPLE_TOKEN_HPP_

#include <string>
//#include <map>
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class simple_token : public parser_if {
public:
	virtual bool parse(std::string_view& message);
	virtual const std::string& get_token() const = 0;
	virtual bool is_value_ok(const std::string_view& value) const;
};

}

}

#endif
