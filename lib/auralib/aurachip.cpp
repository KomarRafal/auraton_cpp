/*
 * aurachip.cpp
 */
#include <algorithm>
#include <regex>

#include "aurachip.hpp"
#include "parser.hpp"

namespace aura
{

bool chip::test() {
	return serial_connection.simple_command(command::compose(command::TEST));
}

void chip::initialize() {
	auto device_str = serial_connection.send_command(command::compose(command::VERSION));
	device_str.append(serial_connection.send_command(command::compose(command::ADDRESS)));
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

bool chip::get_dev_parameters(int32_t dev_id) {
	const auto get_dev_option_cmd = command::compose(command::GET_DEV_OPTION, std::to_string(dev_id));
	auto get_dev_response = serial_connection.send_command(get_dev_option_cmd, device::MAX_PARAMETERS * parameter::MAX_BYTES);
	const auto dev_list = parser::parse_device_list(get_dev_response);
	const auto is_dev_id = (dev_list.count(dev_id) == 1);
	if (!is_dev_id) {
		return false;
	}
	const device get_dev{dev_list.find(dev_id)->second};
	const auto device_element = device_list.find(dev_id);
	if (device_element == device_list.end()) {
		return false;
	}
	auto& my_dev = device_element->second;
	const bool is_device_on_list = (my_dev == get_dev);
	if (!is_device_on_list) {
		return false;
	}
	parameter read_parameter{0};
	while (parser::get_next_parameter(get_dev_response, read_parameter)) {
		my_dev.add_parameter(read_parameter);
	}
	return true;
}

// std::optional could be better here for c++17
bool chip::get_xtal_correction(int32_t& read_value) {
	const std::regex value_regex{"(\\+|-)?\\d+(\r?\n|$)"};
	const auto xtal_response = serial_connection.send_command(command::compose(command::GET_XTAL_CORRECTION));
	const auto xtal_value = parser::parse(xtal_response, parser::VALUE_TOKEN);
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
	return is_clear_ok && parser::check_result(response);
}

int chip::update_device_list() {
	const auto dev_list_str = serial_connection.send_command(command::compose(command::DEV_LIST));
	const auto dev_list_parsed = parser::parse_device_list(dev_list_str);
	device_list.clear();
	for (auto dev_pair : dev_list_parsed) {
		const device dev(dev_pair.second);
		device_list[dev_pair.first] = dev;
	}
	return device_list.size();
}

}
