/*
 * auraton.hpp
 */

#ifndef AURACHIP_HPP
#define AURACHIP_HPP

#include <string>
#include <vector>
#include <map>
#include "auraton.hpp"

namespace aura
{

class cmd {
public:
	enum code {
		TEST,
		VERSION,
		ADDRESS,
		DEV_LIST,
		LAST_CMD
	};

	static const std::string& Get(cmd::code command)
	{
		if (command > LAST_CMD)
			command = LAST_CMD;
		return command_map[command];
	}

private:
	static std::map<const int, const std::string> command_map;
};

class parser {
public:
	typedef std::map<int, std::string> device_list_t;
	static const std::string DEVICE_TOKEN;
	static const std::string EOL;
	static std::string parse(std::string input_str, std::string token);
	static device_list_t parse_device_list(std::string input_str);
};

class version {
public:
	static const std::string PRODUCT_CODE_TOKEN;
	static const std::string FW_VERSION_TOKEN;
	static const std::string HW_VERSION_TOKEN;
	static const std::string MANUFACTURE_CODE_TOKEN;

	explicit version(std::string version_str);

	std::string get_product_code() const
	{
		return product_code;
	}

	std::string get_fw_version() const
	{
		return fw_version;
	}

	std::string get_hw_version() const
	{
		return hw_version;
	}

	std::string get_manufacture_code() const
	{
		return manufacture_code;
	}

	void set_product_code(std::string value)
	{
		product_code = value;
	}

	void set_fw_version(std::string value)
	{
		fw_version = value;
	}

	void set_hw_version(std::string value)
	{
		hw_version = value;
	}

	void set_manufacture_code(std::string value)
	{
		manufacture_code = value;
	}

private:
	std::string product_code;
	std::string fw_version;
	std::string hw_version;
	std::string manufacture_code;
};

class address {
public:
	static const std::string TOKEN;
	explicit address(std::string address_str);

	int get() const {
		return device_address;
	}

	void set(int addr) {
		device_address = addr;
	}

	void set(std::string addr) {
		if (!addr.empty())
		{
			device_address = std::stoul(addr, 0, 16);
		}
		else
		{
			device_address = 0;
		}
	}

private:
	int device_address;
};

class device {
public:
	device() : device_address{""}, device_version{""} {};
	address device_address;
	version device_version;
};

class chip :
	public auraton {
public:

	explicit chip(const std::string& device) :
		auraton{device},
		initialize_flag{false},
		device_address{""},
		device_version{""},
		device_list{}
		{ }

	bool test();
	void initialize();

	bool is_initialize() const {
		return initialize_flag;
	}

	const version get_version() const {
		return device_version;
	}

	const address get_address() const {
		return device_address;
	}

	int update_device_list();
	std::string get_device(int dev_id) {
		return device_list[dev_id];
	}

private:
	bool initialize_flag;
	address device_address;
	version device_version;
	parser::device_list_t device_list;
	const std::string compose_command(cmd::code command) const;
	void initialize_version();
	void initialize_address();

};
}

#endif /* AURACHIP_HPP */
