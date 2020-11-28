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

class chip :
	public auraton {
public:

	explicit chip(const std::string& device) : auraton(device) { }

	bool test();
	std::string get_version();

private:
	const std::string compose_command(cmd::code command) const;
};
}

#endif /* AURACHIP_HPP */
