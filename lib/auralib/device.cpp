/*
 * device.cpp
 */
#include <string_view>
#include <functional>
#include <map>

#include "parsers/commands/version.hpp"
#include "parsers/commands/address.hpp"
#include "parsers/simple_token.hpp"
#include "device.hpp"

namespace aura
{

device::device(const std::string& device_str) :
		product_code(), fw_version(), hw_version(), manufacture_code(), address(0), parameter_map() {
	parser::commands::address address_parser;
	parser::commands::version_pcode pcode_parser;
	parser::commands::version_fw_version fw_version_parser;
	parser::commands::version_hw_version hw_version_parser;
	parser::commands::version_man_code man_code_parser;

	std::map<parser::simple_token*, std::function<void(std::string)>> device_info = {
			{ &address_parser, [this](auto value) { set_address(value); } },
			{ &pcode_parser, [this](auto value) { set_product_code(value); } },
			{ &fw_version_parser, [this](auto value) { set_fw_version(value); }  },
			{ &hw_version_parser, [this](auto value) { set_hw_version(value); }  },
			{ &man_code_parser, [this](auto value) { set_manufacture_code(value); } },
	};

	for (const auto& info : device_info) {
		auto tmp_string = device_str;
		std::string_view device_str_view(tmp_string);
		const auto token = info.first->get_token();
		const auto message = info.first->find(device_str_view, token);
		std::string message_str = message.value_or("");
		std::string_view message_str_view(message_str);
		const auto value = info.first->parse(message_str_view);
		info.second(value.value_or(""));
	}
}

}
