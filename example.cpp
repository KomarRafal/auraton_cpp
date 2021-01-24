/*
 * example.cpp
 */
#include <functional>
#include <iostream>
#include <chrono>
#include <thread>

#include "aurachip.hpp"
#include "connection.hpp"

bool show_device_info(aura::chip& aurachip) {
	if (!aurachip.get_connection().test_uart()) {
		return false;
	}
	aurachip.initialize();
	std::cout << "product_code: " << aurachip.get_product_code() << std::endl;
	std::cout << "fw_version: " << aurachip.get_fw_version() << std::endl;
	std::cout << "hw_version: " << aurachip.get_hw_version() << std::endl;
	std::cout << "manufacture_code: " << aurachip.get_manufacture_code() << std::endl;
	std::cout << "address: 0x" << std::hex << aurachip.get_address() << std::dec << std::endl;
	return true;
}

bool show_parred_devices(aura::chip& aurachip) {
	if (!aurachip.get_connection().test_uart()) {
		return false;
	}
	auto dev_cnt = aurachip.update_device_list();
	std::cout << "Found: " << dev_cnt << " devices" << std::endl;
	for (auto dev : aurachip.device_list) {
		std::cout << "Device: " << dev.first << std::endl;
		std::cout << "product_code: " << dev.second.get_product_code() << std::endl;
		std::cout << "fw_version: " << dev.second.get_fw_version() << std::endl;
		std::cout << "hw_version: " << dev.second.get_hw_version() << std::endl;
		std::cout << "manufacture_code: " << dev.second.get_manufacture_code() << std::endl;
		std::cout << "address: 0x" << std::hex << dev.second.get_address() << std::dec << std::endl;
	}
	return true;
}

bool show_dev_parameters(aura::chip& aurachip) {
	if (!aurachip.get_connection().test_uart()) {
		return false;
	}
	bool status = false;
	const uint16_t max_buffer_size = 10;
	char read_buffer[max_buffer_size];
	std::cout << "Device id:" << std::endl;
	std::cin.get(read_buffer, max_buffer_size);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	const std::string string_buffer{read_buffer};
	if (!string_buffer.empty()) {
		try {
			const auto dev_id = std::stoi(string_buffer);
			std::cout << "Getting device options for dev: " << dev_id << std::endl;
			status = aurachip.update_device_parameters(dev_id);
			if (status) {
				// For C++17 structured bindings can be used:
				// for (const auto& [code, parameter] : aurachip.get_device(dev_id).get_parameters())
				for (const auto& parameter : aurachip.get_device(dev_id).get_parameters()) {
					std::cout << "------------------\n" ;
					std::cout << "CODE: " << parameter.first << std::endl;
					std::cout << "  CHANNEL: " << parameter.second.get_channel() << std::endl;
					std::cout << "  FLAG OWN: " << parameter.second.get_flag_own() << std::endl;
					std::cout << "  FLAG WRITEABLE: " << parameter.second.get_flag_writable() << std::endl;
					std::cout << "  VALUE: " << parameter.second.get_value() << std::endl;
				}
				std::cout << "------------------\n" ;
				std::cout << "Found " << aurachip.get_device(dev_id).get_parameters().size() << " parameters." << std::endl;

			}
		}
		catch (const std::invalid_argument&) {
			std::cout << "Can't convert: " << string_buffer.c_str() << " to number." << std::endl;
		}
	}
	return status;
}

bool handle_xtal_correction(aura::chip& aurachip) {
	if (!aurachip.get_connection().test_uart()) {
		return false;
	}
	bool status = false;
	const uint16_t max_buffer_size = 100;
	char read_buffer[max_buffer_size];
	std::cout << "New crystal correction value (leave empty for reading):" << std::endl;
	std::cin.get(read_buffer, max_buffer_size);
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	const std::string string_buffer{read_buffer};
	if (string_buffer.empty()) {
		int32_t xtal_value;
		const auto is_read_ok = aurachip.get_xtal_correction(xtal_value);
		if (is_read_ok) {
			std::cout << "Crystal correction value: " << xtal_value << std::endl;
		}
		else {
			std::cout << "Can't read crystal value." << std::endl;
		}
	}
	else {
		try {
			const auto xtal_correction = std::stoi(string_buffer);
			std::cout << "setting value: " << xtal_correction << std::endl;
			status = aurachip.set_xtal_correction(xtal_correction);
		}
		catch (const std::invalid_argument&) {
			std::cout << "Can't convert: " << string_buffer.c_str() << " to number." << std::endl;

		}
	}
	return status;
}

using menu_t = std::tuple<const std::string, std::function<bool(aura::chip&)>>;
std::map<char, menu_t> menu = {
		{'o', { "open serial", [](aura::chip& aurachip) { return aurachip.get_connection().open(); } } },
		{'c', { "close serial", [](aura::chip& aurachip)->bool { aurachip.get_connection().close(); return true; } } },
		{'u', { "test serial connection", [](aura::chip& aurachip)->bool { return aurachip.get_connection().test_uart(); } } },
		{'t', { "test radio connection", [](aura::chip& aurachip)->bool { return aurachip.test(); } } },
		{'i', { "show aurachip info", [](aura::chip& aurachip)->bool { return show_device_info(aurachip); } } },
		{'r', { "reset", [](aura::chip& aurachip)->bool { return aurachip.reset(); } } },
		{'f', { "factory reset", [](aura::chip& aurachip)->bool { return aurachip.factory_reset(); } } },
		{'l', { "start linking", [](aura::chip& aurachip)->bool { return aurachip.link(); } } },
		{'d', { "show parred devices", [](aura::chip& aurachip)->bool { return show_parred_devices(aurachip); } } },
		{'p', { "show device options", [](aura::chip& aurachip)->bool { return show_dev_parameters(aurachip); } } },
		{'x', { "read/set crystal correction", [](aura::chip& aurachip)->bool { return handle_xtal_correction(aurachip); } } },
		{'q', { "quit", [](aura::chip&)->bool { exit(0); return true; } } },
};

void show_menu() {
	std::cout << "\n\naurachip options:\n-------------------------------\n" << std::endl;
	for (auto option : menu) {
		std::cout << option.first << ": " << std::get<0>(option.second) << std::endl;
	}
	std::cout << "\n-------------------------------\n" << std::endl;
}

int main(int argc, char **argv) {
	aura::chip my_aura_chip{"COM8"};
	while (true) {
		show_menu();
		const char option = std::cin.get();
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (menu.find(option) != menu.end()) {
			const auto description = std::get<0>(menu[option]);
			const auto action = std::get<1>(menu[option]);
			std::cout << description.c_str() <<std::endl;
			const auto result = action(my_aura_chip);
			std::cout << "Result: " << std::boolalpha << result << std::endl;
		}
		else {
			std::cout << "Unknown option: " << option << std::endl;
		}
	}
	return 0;
}


