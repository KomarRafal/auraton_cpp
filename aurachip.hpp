/*
 * auraton.hpp
 */

#ifndef AURACHIP_HPP
#define AURACHIP_HPP

#include <string>
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
		LAST_CMD
	};

	static const std::string& Get(int command)
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
	static std::string parse(std::string input_str, std::string token);
};

class version {
public:
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

private:
	std::string product_code;
	std::string fw_version;
	std::string hw_version;
	std::string manufacture_code;
};

class address {
public:
	explicit address(std::string address_str);

	int get() const
	{
		return device_address;
	}

private:
	int device_address;
};

class chip :
	public auraton {
public:

	explicit chip(const std::string& device) :
		auraton(device),
		initialize_flag(false),
		device_address(""),
		device_version("")
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

private:
	bool initialize_flag;
	address device_address;
	version device_version;
	const std::string compose_command(cmd::code command) const;
	void initialize_version();
	void initialize_address();
};
}

#endif /* AURACHIP_HPP */
