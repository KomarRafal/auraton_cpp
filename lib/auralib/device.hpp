/*
 * device.hpp
 */

#ifndef DEVICE_HPP_
#define DEVICE_HPP_

#include <parameter.hpp>
#include <cstdint>
#include <string>
#include <map>


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
		 address(0),
		 parameter_map()
	{ }

	explicit device(const std::string& address_str);

	bool operator==(device const& dev) const {
		return (
				(dev.product_code == product_code) &&
				(dev.fw_version == fw_version) &&
				(dev.hw_version == hw_version) &&
				(dev.manufacture_code == manufacture_code) &&
				(dev.address == address)
				);
	}

	void clear() {
		product_code.clear();
		fw_version.clear();
		hw_version.clear();
		manufacture_code.clear();
		address = 0;
	}

	const std::string& get_product_code() const {
		return product_code;
	}

	const std::string& get_fw_version() const {
		return fw_version;
	}

	const std::string& get_hw_version() const {
		return hw_version;
	}

	const std::string& get_manufacture_code() const {
		return manufacture_code;
	}

	int32_t get_address() const {
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

	using parameters_t = std::map<uint32_t, parameter>;
	const parameter& get_paramater(uint32_t code) const {
		return parameter_map.find(code)->second;
	}

	const parameters_t& get_parameters() const {
		return parameter_map;
	}

	void add_parameter(const parameter& param) {
		parameter_map.insert(parameters_t::value_type(param.get_code(), param));
	}

	static const uint16_t MAX_PARAMETERS = 40;
private:
	std::string product_code;
	std::string fw_version;
	std::string hw_version;
	std::string manufacture_code;
	int32_t address;
	parameters_t parameter_map;
};

}
#endif /* DEVICE_HPP_ */
