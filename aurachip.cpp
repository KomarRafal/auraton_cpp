/*
 * aurachip.cpp
 */
#include "aurachip.hpp"

namespace aura
{

std::map<const int, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST" },
		{ cmd::VERSION, "VER"},
		{ cmd::LAST_CMD, ""}
};

version::version(std::string version_str) :
		product_code(), fw_version(), hw_version(), manufacture_code() {
	std::map<std::string, std::string*> version_info = {
			{ "PCODE: ", &product_code },
			{ "FVER: ", &fw_version },
			{ "HVER: ", &hw_version },
			{ "MANCODE: ", &manufacture_code }
	};

	for (auto info : version_info) {
		auto begin = version_str.find(info.first);
		if (begin == std::string::npos)
			continue;
		begin += info.first.length();
		const auto end = version_str.find("\r\n", begin);
		info.second->append(version_str.substr(begin, end - begin));
	}
}

bool chip::test() {
	const auto result = send_command(compose_command(cmd::TEST));
	return (result.compare("OK\r\n") == 0);
}

std::string chip::get_version() {
	const auto version = send_command(compose_command(cmd::VERSION));
	return version;
}

const std::string chip::compose_command(cmd::code command) const {
	std::string composed_command("AT+");
	composed_command.append(cmd::Get(command));
	composed_command.append("?\n");
	return composed_command;
}

}
