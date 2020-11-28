/*
 * example.cpp
 */

#include <iostream>

#include "auraton.hpp"
#include "aurachip.hpp"

int main(int argc, char **argv) {
	aura::chip my_aura_chip("COM8");
	const bool ret = my_aura_chip.open();
	std::cout << "status: " << std::boolalpha << ret << std::endl;
	std::cout << "test_uart: " << my_aura_chip.test_uart() << std::endl;
	std::cout << "test: " << my_aura_chip.test() << std::endl;
	my_aura_chip.initialize();
	std::cout << "product_code: " << my_aura_chip.get_version().get_product_code() << std::endl;
	std::cout << "fw_version: " << my_aura_chip.get_version().get_fw_version() << std::endl;
	std::cout << "hw_version: " << my_aura_chip.get_version().get_hw_version() << std::endl;
	std::cout << "manufacture_code: " << my_aura_chip.get_version().get_manufacture_code() << std::endl;
	std::cout << "address: 0x" << std::hex << my_aura_chip.get_address().get() << std::endl;
	auto dev_cnt = my_aura_chip.update_device_list();
	std::cout << "Found: " << dev_cnt << " devices" << std::endl;
	std::cout << my_aura_chip.get_device(1) << std::endl;
	my_aura_chip.close();
	return 0;
}


