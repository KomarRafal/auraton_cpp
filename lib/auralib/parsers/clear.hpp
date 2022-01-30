/*
 * clear.hpp
 */

#ifndef CLEAR_HPP_
#define CLEAR_HPP_

#include "simple_token.hpp"
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class clear : public simple_token
{
public:
	static const std::string SOURCE_CLEAR_TOKEN;

	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string CLEAR_TOKEN = "PROGRESS:";
};

// TODO:
// - make builder common
class clear_builder
{
public:
	static parser_if::parser_algorithm_t build();
};

}

}

#endif
