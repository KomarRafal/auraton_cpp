/*
 * aurachip.cpp
 */
#include <functional>
#include "aurachip.hpp"

// Test
#include <iostream>

namespace aura
{

std::map<const int, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST" },
		{ cmd::VERSION, "VER" },
		{ cmd::ADDRESS, "ADDR" },
		{ cmd::DEV_LIST, "LIST" },
		{ cmd::LAST_CMD, "" }
};

const std::string parser::DEVICE_TOKEN = "ID: ";
const std::string parser::EOL = "\r\n";

std::string parser::parse(std::string input_str, std::string token) {
	auto begin = input_str.find(token);
	if (begin == std::string::npos)
		return "";
	begin += token.length();
	const auto end = input_str.find(EOL, begin);
	return input_str.substr(begin, end - begin);
}

parser::device_list_t parser::parse_device_list(std::string input_str) {
	device_list_t device_list;
	size_t begin = 0, end;

	while (true)
	{
		begin = input_str.find(DEVICE_TOKEN, begin);
		if (begin == std::string::npos)
			return device_list;
		end = input_str.find(EOL, begin);
		begin += DEVICE_TOKEN.length();
		auto dev_id_str = input_str.substr(begin, end - begin);
		if (dev_id_str.empty())
			return device_list;
		auto dev_id = std::stoul(dev_id_str);
		begin = end + EOL.length();
		end = input_str.find(DEVICE_TOKEN, begin);
		if (device_list.find(dev_id) == device_list.end())
			device_list[dev_id] = input_str.substr(begin, end - begin);
		else
			device_list.find(dev_id)->second.append(input_str.substr(begin, end - begin));

		begin = end;
	}

}

const std::string version::PRODUCT_CODE_TOKEN = "PCODE: ";
const std::string version::FW_VERSION_TOKEN = "FVER: ";
const std::string version::HW_VERSION_TOKEN = "HVER: ";
const std::string version::MANUFACTURE_CODE_TOKEN = "MANCODE: ";

version::version(std::string version_str) :
		product_code(), fw_version(), hw_version(), manufacture_code() {
	std::map<std::string, std::function<void(std::string)>> version_info = {
			{ version::PRODUCT_CODE_TOKEN, [this](auto value) { product_code = value; } },
			{ version::FW_VERSION_TOKEN, [this](auto value) { fw_version = value; }  },
			{ version::HW_VERSION_TOKEN, [this](auto value) { hw_version = value; }  },
			{ version::MANUFACTURE_CODE_TOKEN, [this](auto value) { manufacture_code = value; }  }
	};

	for (const auto info : version_info) {
		info.second(parser::parse(version_str, info.first));
	}
}

const std::string address::TOKEN = "ADDRESS: ";

address::address(std::string address_str) :
		device_address{0} {
	const std::string address = parser::parse(address_str, address::TOKEN);
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

int chip::update_device_list() {
	device dev;
	std::map<std::string, std::function<void(std::string)>> device_info = {
			{ version::PRODUCT_CODE_TOKEN, [&](auto value) { dev.device_version.set_product_code(value); } },
			{ version::FW_VERSION_TOKEN, [&](auto value) { dev.device_version.set_fw_version(value); } },
			{ version::HW_VERSION_TOKEN, [&](auto value) { dev.device_version.set_hw_version(value); } },
			{ version::MANUFACTURE_CODE_TOKEN, [&](auto value) { dev.device_version.set_manufacture_code(value); } },
			{ address::TOKEN, [&](auto value) { dev.device_address.set(value); } }
	};

	// Test ---
	const auto dev_list_str = send_command(compose_command(cmd::DEV_LIST), 10000);
//	const std::string dev_list_str = "ID: 1\r\n"
//					"ADDRESS: 90FC4F9B\r\n"
//					"PCODE: 300d\r\n"
//					"FVER: 1.1\r\n"
//					"HVER: 7\r\n"
//					"MANCODE: 30\r\n"
//					"ID: 2\r\n"
//					"ADDRESS: 456223DA\r\n"
//					"PCODE: 3005\r\n"
//					"FVER: 1.5\r\n"
//					"HVER: 1\r\n"
//					"MANCODE: 30\r\n";
	// ---
	device_list = parser::parse_device_list(dev_list_str);
	return device_list.size();
}

const std::string chip::compose_command(cmd::code command) const {
	std::string composed_command("AT+");
	composed_command.append(cmd::Get(command));
	composed_command.append("?\n");
	return composed_command;
}

}
