/*
 * cmd_test.hpp
 */

#ifndef CMD_TEST_HPP_
#define CMD_TEST_HPP_

#include "simple_token.hpp"
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class cmd_test_radio : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string RADIO_TOKEN = "RADIO:";
};

class cmd_test_flash : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string FLASH_TOKEN = "FLASH:";
};

class cmd_test_backup : public simple_token
{
public:
	virtual const std::string& get_token() const;

private:
	const std::string BACKUP_TOKEN = "BACKUP:";
};

class cmd_test : public parser_if {
public:
	virtual bool parse(std::string_view& message);
};

}

}

#endif
