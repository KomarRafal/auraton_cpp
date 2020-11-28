/*
 * aurachip.cpp
 */
#include "aurachip.hpp"

namespace aura
{

std::map<const int, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST" },
		{ cmd::VERSION, "VER"},
		{ cmd::ADDRESS, "ADDR"},
		{ cmd::LAST_CMD, ""}
};

std::string parser::parse(std::string input_str, std::string token) {
	auto begin = input_str.find(token);
	if (begin == std::string::npos)
		return "";
	begin += token.length();
	const auto end = input_str.find("\r\n", begin);
	return input_str.substr(begin, end - begin);
}

version::version(std::string version_str) :
		product_code(), fw_version(), hw_version(), manufacture_code() {
	std::map<std::string, std::string*> version_info = {
			{ "PCODE: ", &product_code },
			{ "FVER: ", &fw_version },
			{ "HVER: ", &hw_version },
			{ "MANCODE: ", &manufacture_code }
	};

	for (const auto info : version_info) {
		info.second->append(parser::parse(version_str, info.first));
	}
}

address::address(std::string address_str) :
		device_address(0) {
	const std::string address_token = "ADDRESS: ";
	const std::string address = parser::parse(address_str, address_token);
	if (!address.empty())
	{
		device_address = std::stoul(address, 0, 16);
	}
}

bool chip::test() {
	const auto result = send_command(compose_command(cmd::TEST));
	return (result.compare("OK\r\n") == 0);
}

void chip::initialize_version() {
	const auto version_str = send_command(compose_command(cmd::VERSION));
	version ver(version_str);
	device_version = ver;
}

void chip::initialize_address() {
	const auto address_str = send_command(compose_command(cmd::ADDRESS));
	address addr(address_str);
	device_address = addr;
}

void chip::initialize() {
	initialize_version();
	initialize_address();
	initialize_flag = true;
}

const std::string chip::compose_command(cmd::code command) const {
	std::string composed_command("AT+");
	composed_command.append(cmd::Get(command));
	composed_command.append("?\n");
	return composed_command;
}

}
