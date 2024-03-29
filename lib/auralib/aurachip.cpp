/*
 * aurachip.cpp
 */
#include <algorithm>
#include <regex>

#include "parsers/commands/xtal_correction.hpp"
#include "parsers/parser_executor.hpp"
#include "parsers/simple_numeric.hpp"
#include "parsers/next_device_id.hpp"
#include "parsers/command_parser.hpp"
#include "parsers/commands/test.hpp"
#include "parsers/at_traverse.hpp"
#include "parsers/clear.hpp"
#include "events/link.hpp"
#include "aurachip.hpp"

namespace aura
{

bool chip::test() {
	auto command_result = serial_connection.send_command(command::compose(command::TEST));
	std::string_view command_result_view{command_result};
	const auto is_command_test_ok = parser::parser_executor::execute(
			command_result_view,
			parser::commands::test_builder::build());
	return is_command_test_ok.has_value();
}

void chip::initialize() {
	const auto version_str = serial_connection.send_command(command::compose(command::VERSION));
	std::string_view version_str_view{version_str};
	const auto version = parser::command_parser::parse(version_str_view, command::Get(command::VERSION));
	if (!version.has_value()) {
		initialize_flag = false;
		return;
	}

	const auto address_str = serial_connection.send_command(command::compose(command::ADDRESS));
	std::string_view address_str_view{address_str};
	const auto address = parser::command_parser::parse(address_str_view, command::Get(command::ADDRESS));
	if (!address.has_value()) {
		initialize_flag = false;
		return;
	}

	const std::string device_str(static_cast<std::string>(address_str_view) + static_cast<std::string>(version_str_view));
	std::string_view device_string_view{device_str};
	device dev(device_string_view);
	static_cast<device*>(this)->operator=(dev);
	initialize_flag = true;
}

bool chip::link() {
	const auto link_string = serial_connection.send_command(command::compose(command::LINK));
	std::string_view link_str_view{link_string};
	const auto is_link_ok = parser::command_parser::parse(link_str_view, command::Get(command::LINK));
	if (!is_link_ok.has_value()) {
		return false;
	}
	const auto event_string = serial_connection.read_event(parser::events::link::MAX_BUFFER_BYTES, parser::events::link::LINK_WAIT_MS);
	std::string_view event_string_view{event_string};
	const auto status = aura::parser::parser_executor::execute(
			event_string_view,
			aura::parser::events::link_builder::build());
	return static_cast<bool>(status);
}

chip::device_parameters_t chip::get_local_device(int32_t dev_id, std::string_view& get_dev_response) {
	const auto device_element = device_list.find(dev_id);
	if (device_element == device_list.end()) {
		return device_parameters_t{};
	}
	auto* my_dev = &device_element->second;

	auto device_id = get_next_device(get_dev_response);
	while (device_id.first != dev_id) {
		if (device_id.first == 0) {
			return device_parameters_t{};
		}
		device_id = get_next_device(get_dev_response);
	}

	const auto read_device = device{device_id.second};
	const bool is_device_on_list = (*my_dev == read_device);
	if (!is_device_on_list) {
		return device_parameters_t{};
	}
	return device_parameters_t{my_dev, device_id.second};
}

bool chip::update_device_parameters(int32_t dev_id) {
	const auto get_dev_option_cmd = command::compose(command::GET_DEV_OPTIONS, std::to_string(dev_id));
	auto get_dev_response = serial_connection.send_command(get_dev_option_cmd,
			parser::command_parser::MAX_COMMAND_LENGTH + device::MAX_DEVICE_LENGTH + device::MAX_PARAMETERS * parameter::MAX_PARAM_LENGTH);

	std::string_view get_dev_response_view{get_dev_response};
	const auto is_dev_option = parser::command_parser::parse(get_dev_response_view, command::Get(command::GET_DEV_OPTIONS) + "?");
	if (!is_dev_option) {
		return false;
	}

	auto my_device_parameters = get_local_device(dev_id, get_dev_response_view);
	auto my_device = my_device_parameters.first;
	if (my_device == nullptr) {
		return false;
	}
	auto& device_parameters = my_device_parameters.second;
	for (auto read_parameter = parameter::get_next_parameter(device_parameters);
			read_parameter.has_value();
			read_parameter = parameter::get_next_parameter(device_parameters)) {
		my_device->add_parameter(*read_parameter);
	}
	return true;
}

bool chip::update_device_parameter(int32_t dev_id, int32_t code) {
	const auto attributes = std::to_string(dev_id) + "," + std::to_string(code);
	const auto get_dev_option_cmd = command::compose(command::GET_DEV_OPTION, attributes);
	auto get_dev_response = serial_connection.send_command(get_dev_option_cmd,
			parser::command_parser::MAX_COMMAND_LENGTH + device::MAX_DEVICE_LENGTH + parameter::MAX_PARAM_LENGTH);

	std::string_view get_dev_response_view{get_dev_response};
	const auto is_dev_option = parser::command_parser::parse(get_dev_response_view, command::Get(command::GET_DEV_OPTION) + "?");
	if (!is_dev_option) {
		return false;
	}

	auto my_device_parameters = get_local_device(dev_id, get_dev_response_view);
	auto my_device = my_device_parameters.first;
	if (my_device == nullptr) {
		return false;
	}
	auto& parameters = my_device_parameters.second;
	const auto first_paramter = parameter::get_next_parameter(parameters);
	if (!first_paramter.has_value()) {
		return false;
	}
	const bool is_parameter = (code == first_paramter.value().get_code());
	if (!is_parameter) {
		return false;
	}
	const auto other_paramter = parameter::get_next_parameter(parameters);
	if (other_paramter.has_value()) {
		return false;
	}
	my_device->add_parameter(*first_paramter);
	return true;
}

std::optional<int32_t> chip::get_xtal_correction() {
	const auto xtal_response = serial_connection.send_command(command::compose(command::GET_XTAL_CORRECTION));
	std::string_view xtal_response_view{xtal_response};
	const auto result_xtal_correction = parser::parser_executor::execute(
			xtal_response_view,
			parser::commands::get_xtal_correction_builder::build());
	if (result_xtal_correction.has_value()) {
		return std::stoi(*result_xtal_correction);
	}
	return std::nullopt;
}

bool chip::set_xtal_correction(int32_t value) {
	const auto xtal_response = serial_connection.send_command(command::compose(command::SET_XTAL_CORRECTION, std::to_string(value)), 100, 100);
	std::string_view xtal_response_view{xtal_response};
	const auto result_xtal_correction = parser::parser_executor::execute(
			xtal_response_view,
			parser::commands::set_xtal_correction_builder::build());
	if (result_xtal_correction.has_value()) {
		return true;
	}
	return false;
}

bool chip::reset() {
	const auto reset_response = serial_connection.send_command(command::compose(command::RESET));
	std::string_view reset_response_view{reset_response};
	const auto is_reset_ok = parser::command_parser::parse(reset_response_view, command::Get(command::RESET));
	return is_reset_ok.has_value();
}

bool chip::factory_reset() {
	const auto response = serial_connection.send_command(command::compose(command::FACTORY_RESET), FACTORY_RESET_BUFFER_SIZE, FACTORY_RESET_WAIT_MS);
	std::string_view reset_response_view{response};

	parser::at_traverse response_traverse{ reset_response_view };
	auto at_reset_block = response_traverse.get_next_block();
	if (!at_reset_block.has_value()) {
		return false;
	}
	std::string_view factory_reset_block_view{ at_reset_block.value() };
	const auto is_reset_ok = parser::command_parser::parse(factory_reset_block_view, command::Get(command::FACTORY_RESET));
	if (!is_reset_ok.has_value()) {
		return false;
	}

	for (uint32_t percent = 1; percent <= 100; percent++) {
		auto at_clear_block = response_traverse.get_next_block();
		if (!at_clear_block.has_value()) {
			return false;
		}
		std::string_view clear_block_view{ at_clear_block.value() };
		const auto percent_str = aura::parser::parser_executor::execute(
				clear_block_view,
				aura::parser::clear_builder::build());
		const auto is_percent_ok = (std::to_string(percent) == percent_str.value_or(""));
		if (!is_percent_ok) {
			return false;
		}
	}
	return true;
}

chip::device_id_t chip::get_next_device(std::string_view& message) {
	parser::next_device_id devices_parser;
	auto device_message_view = message;
	auto device_id_str = devices_parser.parse(message);
	while (device_id_str.has_value()) {
		std::string_view device_id_string_view{*device_id_str};
		aura::parser::simple_numeric device_id_parser{devices_parser.get_token()};
		auto id_str = device_id_parser.parse(device_id_string_view);
		if (!id_str.has_value()) {
			device_message_view = message;
			device_id_str = devices_parser.parse(message);
			continue;
		}
		const auto id = std::stoi(*id_str);
		(void)device_id_parser.parse(device_message_view);
		const auto suffix_size = message.size();
		device_message_view.remove_suffix(suffix_size);
		return device_id_t{id, device_message_view};
	}
	return device_id_t{};
}

int chip::update_device_list() {
	const auto device_list_str = serial_connection.send_command(command::compose(command::DEV_LIST));
	std::string_view device_list_string_view{device_list_str};
	const auto is_dev_list = parser::command_parser::parse(device_list_string_view, command::Get(command::DEV_LIST));
	if (!is_dev_list) {
		return 0;
	}
	device_list.clear();
	auto device_id = get_next_device(device_list_string_view);
	while (device_id.first != 0) {
		device_list[device_id.first] = device{device_id.second};
		device_id = get_next_device(device_list_string_view);
	}
	return device_list.size();
}

}
