/*
 * cmd_test.cpp
 */

#include "cmd_test.hpp"

namespace aura
{

namespace parser
{

const std::string& cmd_test_radio::get_token() const
{
	return RADIO_TOKEN;
}

const simple_token<ok_error_type>::token_map& cmd_test_radio::get_token_map() const
{
	return ok_error.get_token_map();
}

const std::string& cmd_test_flash::get_token() const
{
	return FLASH_TOKEN;
}

const simple_token<ok_error_type>::token_map& cmd_test_flash::get_token_map() const
{
	return ok_error.get_token_map();
}

const std::string& cmd_test_backup::get_token() const
{
	return BACKUP_TOKEN;
}

const simple_token<ok_error_type>::token_map& cmd_test_backup::get_token_map() const
{
	return ok_error.get_token_map();
}

std::optional<cmd_test_type> cmd_test::parse(std::string_view& message)
{
	const auto radio_status = radio_parser.parse(message);
	if (!radio_status || (radio_status.value() != ok_error_type::OK)) {
		return cmd_test_type::RADIO_ERROR;
	}
	const auto flash_status = flash_parser.parse(message);
	if (!flash_status || (flash_status.value() != ok_error_type::OK)) {
		return cmd_test_type::FLASH_ERROR;
	}
	const auto backup_status = backup_parser.parse(message);
	if (!backup_status || (backup_status.value() != ok_error_type::OK)) {
		return cmd_test_type::BACKUP_ERROR;
	}
	return cmd_test_type::TEST_OK;
}

}

}
