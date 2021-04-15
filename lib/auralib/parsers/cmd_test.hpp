/*
 * cmd_test.hpp
 */

#ifndef CMD_TEST_HPP_
#define CMD_TEST_HPP_

#include <memory>
#include "simple_token.hpp"
#include "parser_if.hpp"
#include "command.hpp"

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

class test_command : public command {
public:
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string TEST_COMMAND = "TEST?";
};

class test : public parser_if {
public:
	virtual bool parse(std::string_view& message);
	static std::vector<std::unique_ptr<parser_if>> parser_builder();
};

}

}

}

#endif
