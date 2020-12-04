/*
 * device.hpp
 */

#ifndef DEVICE_HPP_
#define DEVICE_HPP_

#include <cstdint>
#include <string>

namespace aura
{

class device {
public:
	static const std::string PRODUCT_CODE_TOKEN;
	static const std::string FW_VERSION_TOKEN;
	static const std::string HW_VERSION_TOKEN;
	static const std::string MANUFACTURE_CODE_TOKEN;
	static const std::string ADDRESS_TOKEN;

	device() :
		 product_code(),
		 fw_version(),
		 hw_version(),
		 manufacture_code(),
		 address(0)
	{ }

	explicit device(const std::string& address_str);

	void clear() {
		product_code.clear();
		fw_version.clear();
		hw_version.clear();
		manufacture_code.clear();
		address = 0;
	}

	std::string get_product_code() {
		return product_code;
	}

	std::string get_fw_version() {
		return fw_version;
	}

	std::string get_hw_version() {
		return hw_version;
	}

	std::string get_manufacture_code() {
		return manufacture_code;
	}

	int32_t get_address() {
		return address;
	}

	void set_product_code(const std::string& value)
	{
		product_code = value;
	}

	void set_fw_version(const std::string& value)
	{
		fw_version = value;
	}

	void set_hw_version(const std::string& value)
	{
		hw_version = value;
	}

	void set_manufacture_code(const std::string& value)
	{
		manufacture_code = value;
	}

	void set_address(int addr) {
		address = addr;
	}

	void set_address(const std::string& addr) {
		if (!addr.empty())
		{
			address = std::stoul(addr, 0, 16);
		}
		else
		{
			address = 0;
		}
	}

private:
	std::string product_code;
	std::string fw_version;
	std::string hw_version;
	std::string manufacture_code;
	int32_t address;
};

}
#endif /* DEVICE_HPP_ */
