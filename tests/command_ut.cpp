/*
 * command_ut.cpp
 */
#include <string>
#include <map>

#include "gtest/gtest.h"
#include "command.hpp"

using namespace aura;

TEST(cmd_ut, compose)
{
	const std::map<command::code, std::string> commands_pattern = {
			{ command::TEST, "AT+TEST?" },
			{ command::VERSION, "AT+VER?" },
			{ command::LINK, "AT+LINK" },
			{ command::UNLINK, "AT+UNLINK" },
			{ command::ADDRESS, "AT+ADDR?" },
			{ command::DEV_LIST, "AT+LIST?" },
			{ command::FACTORY_RESET, "AT+MSTRST" },
			{ command::GET_XTAL_CORRECTION, "AT+CRYSTALCORRECTION?" },
			{ command::SET_XTAL_CORRECTION, "AT+CRYSTALCORRECTION" },
			{ command::RESET, "AT+RST" },
	};

	for (auto command : commands_pattern) {
		EXPECT_EQ(command::compose(command.first), command.second.append("\n"));
	}
	EXPECT_TRUE(command::compose(command::LAST_CMD).empty());
}

TEST(cmd_ut, compose_attribute)
{
	const std::map<command::code, std::pair<std::string, std::string>> commands_pattern = {
			{ command::SET_XTAL_CORRECTION, {"-70000", "AT+CRYSTALCORRECTION=-70000"} },
	};
	for (auto command : commands_pattern) {
		EXPECT_EQ(command::compose(command.first, std::get<0>(command.second)), std::get<1>(command.second).append("\n"));
	}
}
