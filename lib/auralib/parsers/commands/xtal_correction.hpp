/*
 * xtal_correction.hpp
 */

#ifndef COMMANDS_XTAL_CORRECTION_HPP_
#define COMMANDS_XTAL_CORRECTION_HPP_

#include "simple_token.hpp"
#include "parser_if.hpp"

/*
AT:START
SOURCE:COMMAND
COMMAND: CRYSTALCORRECTION?
STATUS:OK
VALUE: -70000
AT:STOP
*/

namespace aura
{

namespace parser
{

namespace commands
{

// TODO:
// - make builder common
class xtal_correction : public simple_token
{
public:
	static const std::string get_command_token();
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	static const std::string COMMAND_XTAL_CORRECTION_TOKEN;
	const std::string XTAL_CORRECTION_TOKEN = "VALUE: ";
};

class xtal_correction_builder
{
public:
	static parser_if::parser_algorithm_t build();
};

}

}

}

#endif
