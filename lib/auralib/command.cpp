/*
 * command.cpp
 */
#include "command.hpp"

namespace aura
{

std::map<const command::code, const std::string> command::command_map = {
		{ command::TEST, "TEST?" },
		{ command::VERSION, "VER?" },
		{ command::LINK, "LINK" },
		{ command::UNLINK, "UNLINK" },
		{ command::ADDRESS, "ADDR?" },
		{ command::DEV_LIST, "LIST?" },
		{ command::FACTORY_RESET, "MSTRST" },
		{ command::RESET, "RST" },
		{ command::EVENT_LINK, "EVENT_LINK" },
		{ command::XTAL_CORRECTION, "CRYSTALCORRECTION?" },
		{ command::LAST_CMD, "" }
};

const std::string command::compose(const command::code& command_code) {
	if (command_code >= command::code::LAST_CMD) {
		return std::string();
	}
	std::string composed_command("AT+");
	composed_command.append(Get(command_code));
	composed_command.append("\n");
	return composed_command;
}

}

