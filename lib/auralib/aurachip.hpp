/*
 * auraton.hpp
 */

#ifndef AURACHIP_HPP
#define AURACHIP_HPP

#include <string>
#include <vector>
#include <map>

#include "connection.hpp"
#include "device.hpp"

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

	static const std::string compose(cmd::code command) {
		std::string composed_command("AT+");
		composed_command.append(cmd::Get(command));
		composed_command.append("?\n");
		return composed_command;
	}

private:
	static std::map<const int, const std::string> command_map;
};

class chip :
	public connection,
	public device {
public:

	explicit chip(const std::string& device_port) :
		connection{device_port},
		device{},
		initialize_flag{false},
		device_list{}
		{ }

	bool test();
	void initialize();

	bool is_initialize() const {
		return initialize_flag;
	}

	int update_device_list();
	device get_device(int dev_id) {
		return device_list[dev_id];
	}

	std::map<int, device> device_list;

private:
	bool initialize_flag;
	const std::string compose_command(cmd::code command) const;
	void initialize_version();
	void initialize_address();

};
}

#endif /* AURACHIP_HPP */
