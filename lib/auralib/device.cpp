/*
 * device.cpp
 */
#include <string_view>
#include <functional>
#include <utility>
#include <vector>

#include "parsers/simple_token.hpp"
#include "parsers/version.hpp"
#include "parsers/address.hpp"
#include "device.hpp"

namespace aura
{

/*
ADDRESS: 30090005
PCODE: 300c
FVER: 2.4
HVER: 1.0
MANCODE: 30
*/

device::device(std::string_view& device_str_view) :
		product_code(), fw_version(), hw_version(), manufacture_code(), address(0), parameter_map() {
	parser::address address_parser;
	parser::version_pcode pcode_parser;
	parser::version_fw_version fw_version_parser;
	parser::version_hw_version hw_version_parser;
	parser::version_man_code man_code_parser;

	std::vector<std::pair<parser::parser_if*, std::function<void(std::string)>>> device_info_reader = {
			{ &address_parser, [this](auto value) { set_address(value); } },
			{ &pcode_parser, [this](auto value) { set_product_code(value); } },
			{ &fw_version_parser, [this](auto value) { set_fw_version(value); }  },
			{ &hw_version_parser, [this](auto value) { set_hw_version(value); }  },
			{ &man_code_parser, [this](auto value) { set_manufacture_code(value); } },
	};

	for (const auto& info : device_info_reader) {
		const auto value = info.first->parse(device_str_view);
		info.second(value.value_or(""));
	}
}

}
