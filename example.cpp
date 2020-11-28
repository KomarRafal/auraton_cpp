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
	std::cout << "test_uart: " << my_aura_chip.test_uart() << std::endl;;
	std::cout << "test: " << my_aura_chip.test() << std::endl;
	const std::string version = my_aura_chip.get_version();
	std::cout << "Version:\n" << version << std::endl;
	const aura::version aura_ver(version);
	std::cout << "product_code: " << aura_ver.get_product_code() << std::endl;
	std::cout << "fw_version: " << aura_ver.get_fw_version() << std::endl;
	std::cout << "hw_version: " << aura_ver.get_hw_version() << std::endl;
	std::cout << "manufacture_code: " << aura_ver.get_manufacture_code() << std::endl;
	my_aura_chip.close();
	return 0;
}


