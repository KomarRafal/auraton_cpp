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
	if (!is_cmd_ok)
		return false;
	const bool is_event = serial_connection.wait_for_read(LINK_WAIT_MS);
	if (!is_event)
		return false;
	const bool is_event_ok = serial_connection.check_event(command::Get(command::EVENT_LINK));
	return is_event_ok;
}

int32_t chip::get_xtal_correction() {
	const auto xtal_response = serial_connection.send_command(command::compose(command::XTAL_CORRECTION));
	const auto xtal_value = parser::parse(xtal_response, parser::VALUE_TOKEN);
	return std::stoi(xtal_value);
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
	// TODO: there is a bug in aurachip:
	//       missing a newline character after "100%".
	//       After fix regex should be: "\\Clear:\\d+%(?=\r?\n|$)"
	const std::regex clear_regex{"\\Clear:\\d+%(?=\r?\n|\r|$)"};
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
