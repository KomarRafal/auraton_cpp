/*
 * cmd_test.cpp
 */

#include <vector>
#include "cmd_test.hpp"

namespace aura
{

namespace parser
{

const std::string& cmd_test_radio::get_token() const
{
	return RADIO_TOKEN;
}

const std::string& cmd_test_flash::get_token() const
{
	return FLASH_TOKEN;
}

const std::string& cmd_test_backup::get_token() const
{
	return BACKUP_TOKEN;
}

bool cmd_test::parse(std::string_view& message)
{
	/*const*/ cmd_test_radio radio_parser;
	/*const*/ cmd_test_flash flash_parser;
	/*const*/ cmd_test_backup backup_parser;
	const std::vector<parser_if*> cmd_test_parser = {
			&radio_parser,
			&flash_parser,
			&backup_parser
	};
	for (auto parser : cmd_test_parser) {
		if (parser->parse(message) == false) {
			return false;
		}
	}
	return true;
}

}

}
