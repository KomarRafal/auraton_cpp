/*
 * example.cpp
 */

#include <iostream>
#include <chrono>
#include <thread>

#include "aurachip.hpp"
#include "connection.hpp"

int main(int argc, char **argv) {
	aura::chip my_aura_chip("COM8");
	const bool ret = my_aura_chip.get_connection().open();
	std::cout << "status: " << std::boolalpha << ret << std::endl;
	std::cout << "reset: " << my_aura_chip.reset() << std::endl;
	std::cout << "test_uart: " << my_aura_chip.get_connection().test_uart() << std::endl;
	std::cout << "test: " << my_aura_chip.test() << std::endl;
	my_aura_chip.initialize();
	std::cout << "product_code: " << my_aura_chip.get_product_code() << std::endl;
	std::cout << "fw_version: " << my_aura_chip.get_fw_version() << std::endl;
	std::cout << "hw_version: " << my_aura_chip.get_hw_version() << std::endl;
	std::cout << "manufacture_code: " << my_aura_chip.get_manufacture_code() << std::endl;
	std::cout << "address: 0x" << std::hex << my_aura_chip.get_address() << std::endl;
	std::cout << "Start linking..." << std::endl;
	std::cout << my_aura_chip.link() << std::endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	auto dev_cnt = my_aura_chip.update_device_list();
	std::cout << "Found: " << dev_cnt << " devices" << std::endl;
	for (auto dev : my_aura_chip.device_list) {
		std::cout << "Device: " << dev.first << std::endl;
		std::cout << "product_code: " << dev.second.get_product_code() << std::endl;
		std::cout << "fw_version: " << dev.second.get_fw_version() << std::endl;
		std::cout << "hw_version: " << dev.second.get_hw_version() << std::endl;
		std::cout << "manufacture_code: " << dev.second.get_manufacture_code() << std::endl;
		std::cout << "address: 0x" << std::hex << dev.second.get_address() << std::endl;
	}
	my_aura_chip.get_connection().close();
	return 0;
}


