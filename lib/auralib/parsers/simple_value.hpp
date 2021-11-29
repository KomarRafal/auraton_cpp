/*
 * simple_value.hpp
 */

#ifndef SIMPLE_VALUE_HPP_
#define SIMPLE_VALUE_HPP_

#include <string>

#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class simple_value : public parser_if {
public:
	virtual parsed_value parse(std::string_view& message);
	virtual const std::string& get_token() const = 0;
	virtual bool is_value_ok(const std::string_view& value) const = 0;
};

}

}

#endif
