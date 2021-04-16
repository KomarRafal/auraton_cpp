/*
 * test.hpp
 */

#ifndef COMMAND_TEST_HPP_
#define COMMAND_TEST_HPP_

#include "parsers/simple_token.hpp"
#include "parsers/parser_if.hpp"
#include "builder_if.hpp"

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
	virtual bool parse(std::string_view& message);
	static const std::string get_command_token();
private:
	static const std::string COMMAND_TEST_TOKEN;
};

class test_builder : public builder_if
{
public:
	virtual builder_t build() const;
};

}

}

}

#endif
