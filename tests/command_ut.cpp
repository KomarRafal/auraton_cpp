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
			{ command::RESET, "AT+RST" },
	};

	for (auto command : commands_pattern) {
		EXPECT_EQ(command::compose(command.first), command.second.append("\n"));
	}
	EXPECT_TRUE(command::compose(command::LAST_CMD).empty());
}
