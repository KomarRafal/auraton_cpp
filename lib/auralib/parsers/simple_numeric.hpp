/*
 * simple_numeric.hpp
 */

#ifndef SIMPLE_NUMERIC_HPP_
#define SIMPLE_NUMERIC_HPP_

#include "simple_token.hpp"

namespace aura
{

namespace parser
{

namespace commands
{


class simple_numeric : public simple_token
{
public:
	simple_numeric(const std::string& specific_token);
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	std::string token;
};

}

}

}

#endif
