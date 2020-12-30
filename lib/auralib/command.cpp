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
		{ command::GET_DEV_OPTION, "GETDEVOPTION" },
		{ command::GET_XTAL_CORRECTION, "CRYSTALCORRECTION?" },
		{ command::SET_XTAL_CORRECTION, "CRYSTALCORRECTION" },
		{ command::LAST_CMD, "" }
};

const std::string command::compose(const command::code& command_code, const std::string& attributes) {
	if (command_code >= command::code::LAST_CMD) {
		return std::string();
	}
	std::string composed_command("AT+");
	composed_command.append(Get(command_code));
	if (!attributes.empty()) {
		composed_command.append("=");
		composed_command.append(attributes);
	}
	composed_command.append("\n");
	return composed_command;
}

}

