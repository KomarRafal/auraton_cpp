/*
 * xtal_correction.hpp
 */

#ifndef COMMANDS_XTAL_CORRECTION_HPP_
#define COMMANDS_XTAL_CORRECTION_HPP_

#include "simple_token.hpp"
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

class xtal_correction : public simple_token
{
public:
	static const std::string get_command_token();
	static const std::string get_set_command_token();
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	static const std::string COMMAND_GET_XTAL_CORRECTION_TOKEN;
	static const std::string COMMAND_SET_XTAL_CORRECTION_TOKEN;
	const std::string XTAL_CORRECTION_TOKEN = "VALUE: ";
};

// TODO:
// - make builder common
class get_xtal_correction_builder
{
public:
	static parser_if::parser_algorithm_t build();
};

class set_xtal_correction_builder
{
public:
	static parser_if::parser_algorithm_t build();
};

}

}

}

#endif
