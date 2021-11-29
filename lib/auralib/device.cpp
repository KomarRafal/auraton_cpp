/*
 * device.cpp
 */
#include <functional>
#include <regex>
#include <map>

#include "parser.hpp"
#include "device.hpp"

// TBR
#include <iostream>

namespace aura
{

const std::string device::PRODUCT_CODE_TOKEN = "PCODE: ";
const std::string device::FW_VERSION_TOKEN = "FVER: ";
const std::string device::HW_VERSION_TOKEN = "HVER: ";
const std::string device::MANUFACTURE_CODE_TOKEN = "MANCODE: ";
const std::string device::ADDRESS_TOKEN = "ADDRESS: ";


device::device(const std::string& device_str) :
		product_code(), fw_version(), hw_version(), manufacture_code(), address(0), parameter_map() {
	std::map<std::string, std::function<void(std::string)>> device_info = {
			{ device::PRODUCT_CODE_TOKEN, [this](auto value) { set_product_code(value); } },
			{ device::FW_VERSION_TOKEN, [this](auto value) { set_fw_version(value); }  },
			{ device::HW_VERSION_TOKEN, [this](auto value) { set_hw_version(value); }  },
			{ device::MANUFACTURE_CODE_TOKEN, [this](auto value) { set_manufacture_code(value); } },
			{ device::ADDRESS_TOKEN, [this](auto value) { set_address(value); } },
	};

	for (const auto& info : device_info) {
		info.second(parser_legacy::parse(device_str, info.first));
	}
//	const uint8_t TOKENS_NUMBER = 5u;
//	const std::regex split_regex{"(.+)(?:|$)"};
//    auto lines_begin =  std::sregex_iterator(device_str.begin(), device_str.end(), split_regex);
//    auto lines_end = std::sregex_iterator();
//    if (std::distance(lines_begin, lines_end) != TOKENS_NUMBER) {
//    	// TODO: throw?
//    	return;
//    }
//    for (std::sregex_iterator it = lines_begin; it != lines_end; ++it) {
//           std::smatch line = *it;
//           std::string line_str = line.str();
//           std::cout << line_str << " --- \n";
//    }
}

}
