/*
 * aurachip.cpp
 */
#include <algorithm>
#include <regex>

#include "parsers/command_parser.hpp"
#include "aurachip.hpp"
#include "parser.hpp"

namespace aura
{

bool chip::test() {
	return serial_connection.simple_command(command::compose(command::TEST));
}

void chip::initialize() {
	const auto version_str = serial_connection.send_command(command::compose(command::VERSION));
	std::string_view version_str_view(version_str);
	const auto is_version = parser::command_parser::parse(version_str_view);
	if (!is_version) {
		initialize_flag = false;
		return;
	}
	std::string device_str(static_cast<std::string>(version_str_view));

	const auto address_str = serial_connection.send_command(command::compose(command::ADDRESS));
	std::string_view address_str_view(address_str);
	const auto is_address = parser::command_parser::parse(address_str_view);
	if (!is_address) {
		initialize_flag = false;
		return;
	}
	device_str.append(static_cast<std::string>(address_str_view));
	device dev(device_str);
	static_cast<device*>(this)->operator=(dev);
	initialize_flag = true;
}

bool chip::link() {
	const bool is_cmd_ok = serial_connection.simple_command(command::compose(command::LINK));
	if (!is_cmd_ok) {
		return false;
	}
	const bool is_event = serial_connection.wait_for_read(LINK_WAIT_MS);
	if (!is_event) {
		return false;
	}
	const bool is_event_ok = serial_connection.check_event(command::Get(command::EVENT_LINK));
	return is_event_ok;
}

device* chip::get_local_device(int32_t dev_id, std::string get_dev_response) {
	const auto dev_list = parser_legacy::parse_device_list(get_dev_response);
	const auto is_dev_id = (dev_list.count(dev_id) == 1);
	if (!is_dev_id) {
		return nullptr;
	}
	const device get_dev{dev_list.find(dev_id)->second};
	const auto device_element = device_list.find(dev_id);
	if (device_element == device_list.end()) {
		return nullptr;
	}
	auto* my_dev = &device_element->second;
	const bool is_device_on_list = (*my_dev == get_dev);
	if (!is_device_on_list) {
		return nullptr;
	}
	return my_dev;
}

bool chip::update_device_parameters(int32_t dev_id) {
	const auto get_dev_option_cmd = command::compose(command::GET_DEV_OPTIONS, std::to_string(dev_id));
	auto get_dev_response = serial_connection.send_command(get_dev_option_cmd,
			device::MAX_DEVICE_LENGTH + device::MAX_PARAMETERS * parameter::MAX_PARAM_LENGTH);
	device* my_device = get_local_device(dev_id, get_dev_response);
	if (my_device == nullptr) {
		return false;
	}
	parameter read_parameter{0};
	while (parser_legacy::get_next_parameter(get_dev_response, read_parameter)) {
		my_device->add_parameter(read_parameter);
	}
	return true;
}

bool chip::update_device_parameter(int32_t dev_id, int32_t code) {
	const auto attributes = std::to_string(dev_id) + "," + std::to_string(code);
	const auto get_dev_option_cmd = command::compose(command::GET_DEV_OPTION, attributes);
	auto get_dev_response = serial_connection.send_command(get_dev_option_cmd,
			device::MAX_DEVICE_LENGTH + parameter::MAX_PARAM_LENGTH);
	device* my_device = get_local_device(dev_id, get_dev_response);
	if (my_device == nullptr) {
		return false;
	}
	parameter read_parameter{0};
	const bool is_first_paramter = parser_legacy::get_next_parameter(get_dev_response, read_parameter);
	if (!is_first_paramter) {
		return false;
	}
	const bool is_parameter = (code == read_parameter.get_code());
	if (!is_parameter) {
		return false;
	}
	const bool is_other_paramter = parser_legacy::get_next_parameter(get_dev_response, read_parameter);
	if (is_other_paramter) {
		return false;
	}
	my_device->add_parameter(read_parameter);
	return true;
}

// std::optional could be better here for c++17
bool chip::get_xtal_correction(int32_t& read_value) {
	const std::regex value_regex{"(\\+|-)?\\d+(\r?\n|$)"};
	const auto xtal_response = serial_connection.send_command(command::compose(command::GET_XTAL_CORRECTION));
	const auto xtal_value = parser_legacy::parse(xtal_response, parser_legacy::VALUE_TOKEN);
	if (xtal_value.empty() ||
			!std::regex_match(xtal_value, std::regex(value_regex))) {
		return false;
	}
	read_value = std::stoi(xtal_value);
	return true;
}

bool chip::set_xtal_correction(int32_t value) {
	return serial_connection.simple_command(command::compose(command::SET_XTAL_CORRECTION, std::to_string(value)), 100);
}

bool chip::reset() {
	return serial_connection.simple_command(command::compose(command::RESET));
}

bool chip::factory_reset() {
	const auto response = serial_connection.send_command(command::compose(command::FACTORY_RESET), FACTORY_RESET_BUFFER_SIZE, FACTORY_RESET_WAIT_MS);
	// Response should be:
	// Clear:1%
	// Clear:2%
	// ...
	// Clear:99%
	// Clear:100%
	// CheckIfPerformBackup, actual firmware is in 1 half
	// OK
	//
	// So let's check clearing progress first.
	const std::regex clear_regex{"\\Clear:\\d+%(?=\r?\n|$)"};
	const auto regex_begin = std::sregex_iterator(response.begin(), response.end(), clear_regex);
	const auto regex_end = std::sregex_iterator();
	const auto clear_log_count = std::distance(regex_begin, regex_end);
	const bool is_clear_ok = (clear_log_count == 100);
	return is_clear_ok && parser_legacy::check_result(response);
}

int chip::update_device_list() {
	const auto dev_list_str = serial_connection.send_command(command::compose(command::DEV_LIST));
	const auto dev_list_parsed = parser_legacy::parse_device_list(dev_list_str);
	device_list.clear();
	for (auto dev_pair : dev_list_parsed) {
		const device dev(dev_pair.second);
		device_list[dev_pair.first] = dev;
	}
	return device_list.size();
}

}
