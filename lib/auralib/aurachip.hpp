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
#include "command.hpp"
#include "device.hpp"

namespace aura
{

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
	int32_t get_xtal_correction();
	static const uint32_t LINK_WAIT_MS = 32000;
	bool reset();
	static const uint16_t FACTORY_RESET_BUFFER_SIZE = 1500;
	static const uint32_t FACTORY_RESET_WAIT_MS = 22000;
	bool factory_reset();

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
	void initialize_version();
	void initialize_address();

};
}

#endif /* AURACHIP_HPP */
