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

enum ok_error_type {
	OK,
	ERROR,
};

class cmd_test_ok_error
{
public:
	const simple_token<ok_error_type>::token_map& get_token_map() const {
		return ok_error_map;
	}

private:
	const simple_token<ok_error_type>::token_map ok_error_map = {
			{ "OK", ok_error_type::OK },
			{ "ERROR", ok_error_type::ERROR },
	};
};

class cmd_test_radio : public simple_token<ok_error_type>
{
public:
	virtual const std::string& get_token() const;
	virtual const simple_token<ok_error_type>::token_map& get_token_map() const;

private:
	const std::string RADIO_TOKEN = "RADIO:";
	const cmd_test_ok_error ok_error;
};

class cmd_test_flash : public simple_token<ok_error_type>
{
public:
	virtual const std::string& get_token() const;
	virtual const simple_token<ok_error_type>::token_map& get_token_map() const;

private:
	const std::string FLASH_TOKEN = "FLASH:";
	const cmd_test_ok_error ok_error;
};

class cmd_test_backup : public simple_token<ok_error_type>
{
public:
	virtual const std::string& get_token() const;
	virtual const simple_token<ok_error_type>::token_map& get_token_map() const;

private:
	const std::string BACKUP_TOKEN = "BACKUP:";
	const cmd_test_ok_error ok_error;
};

enum cmd_test_type {
	TEST_OK,
	RADIO_ERROR,
	FLASH_ERROR,
	BACKUP_ERROR,
};

class cmd_test : public parser_if<cmd_test_type> {
public:
	virtual std::optional<cmd_test_type> parse(std::string_view& message);
private:
	cmd_test_radio radio_parser;
	cmd_test_flash flash_parser;
	cmd_test_backup backup_parser;
};

}

}

#endif
