/*
 * auraton.hpp
 */

#ifndef AURACHIP_HPP
#define AURACHIP_HPP

#include <cstdint>
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
		LINK,
		EVENT_LINK,
		ADDRESS,
		DEV_LIST,
		RESET,
		LAST_CMD
	};

	static const std::string& Get(cmd::code command)
	{
		if (command > LAST_CMD)
			command = LAST_CMD;
		return command_map[command];
	}

	static const std::string compose(const cmd::code& command);

private:
	static std::map<const uint32_t, const std::string> command_map;
};

class chip :
	public device {
public:

	explicit chip(const std::string& device_port) :
		device{},
		device_list{},
		serial_connection{device_port},
		initialize_flag{false}
		{ }

	bool test();
	void initialize();
	bool link();
	static const uint32_t LINK_WAIT_MS = 32000;
	bool reset();

	bool is_initialize() const {
		return initialize_flag;
	}

	int update_device_list();
	const device& get_device(uint32_t dev_id) {
		return device_list[dev_id];
	}

	connection& get_connection() {
		return serial_connection;
	}

	std::map<int, device> device_list;

private:
	connection serial_connection;
	bool initialize_flag;
	const std::string compose_command(const cmd::code& command) const;
	void initialize_version();
	void initialize_address();

};
}

#endif /* AURACHIP_HPP */
