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
	std::cout << "address: 0x" << std::hex << aurachip.get_address() << std::endl;
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
		std::cout << "address: 0x" << std::hex << dev.second.get_address() << std::endl;
	}
	return true;
}

bool show_xtal_correction(aura::chip& aurachip) {
	if (!aurachip.get_connection().test_uart()) {
		return false;
	}
	const auto xtal_value = aurachip.get_xtal_correction();
	std::cout << "Cristal correction value: " << xtal_value << std::endl;
	return true;
}

using menu_t = std::tuple<const std::string, std::function<bool(aura::chip&)>>;
std::map<char, menu_t> menu = {
		{'o', { "open serial", [](aura::chip& aurachip) { return aurachip.get_connection().open(); } } },
		{'c', { "close serial", [](aura::chip& aurachip)->bool { aurachip.get_connection().close(); return true; } } },
		{'u', { "test serial connection", [](aura::chip& aurachip)->bool { return aurachip.get_connection().test_uart(); } } },
		{'t', { "test radio connection", [](aura::chip& aurachip)->bool { return aurachip.test(); } } },
		{'r', { "reset", [](aura::chip& aurachip)->bool { return aurachip.reset(); } } },
		{'f', { "factory reset", [](aura::chip& aurachip)->bool { return aurachip.factory_reset(); } } },
		{'l', { "start linking", [](aura::chip& aurachip)->bool { return aurachip.link(); } } },
		{'d', { "show parred devices", [](aura::chip& aurachip)->bool { return show_parred_devices(aurachip); } } },
		{'x', { "read crystal correction", [](aura::chip& aurachip)->bool { return show_xtal_correction(aurachip); } } },
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
		std::cout << option << std::endl;
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


