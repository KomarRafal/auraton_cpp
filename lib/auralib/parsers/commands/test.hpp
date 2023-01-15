/*
 * test.hpp
 */

#ifndef COMMAND_TEST_HPP_
#define COMMAND_TEST_HPP_

#include "simple_token.hpp"
#include "parser_if.hpp"

/*
AT:START
SOURCE:COMMAND
COMMAND: TEST?
STATUS:OK
RADIO:OK
FLASH:OK
BACKUP:OK
AT:STOP
*/
namespace aura
{

namespace parser
{

namespace commands
{

class test_radio : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string RADIO_TOKEN = "RADIO:";
};

class test_flash : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string FLASH_TOKEN = "FLASH:";
};

class test_backup : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string BACKUP_TOKEN = "BACKUP:";
};

class test : public parser_if
{
public:
	virtual parsed_value_t parse(std::string_view& message);
	static const std::string get_command_token();
private:
	static const std::string COMMAND_TEST_TOKEN;
};

class test_builder
{
public:
	static parser_if::parser_algorithm_t build();
};

}

}

}

#endif
