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
	bool update_device_parameters(int32_t dev_id);
	bool update_device_parameter(int32_t dev_id, int32_t code);
	bool get_xtal_correction(int32_t& read_value);
	bool set_xtal_correction(int32_t value);
	static const uint32_t LINK_WAIT_MS = 32000;
	bool reset();
	static const uint16_t FACTORY_RESET_BUFFER_SIZE = 1500;
	static const uint32_t FACTORY_RESET_WAIT_MS = 22000;
	bool factory_reset();

	bool is_initialize() const {
		return initialize_flag;
	}

	int update_device_list();

	const device& get_device(uint32_t dev_id) const {
		return device_list.find(dev_id)->second;
	}

	device& get_device(uint32_t dev_id) {
		return device_list.find(dev_id)->second;
	}

	connection& get_connection() {
		return serial_connection;
	}

	std::map<uint32_t, device> device_list;

private:
	connection serial_connection;
	bool initialize_flag;
	void initialize_version();
	void initialize_address();
	device* get_local_device(int32_t dev_id, std::string get_dev_response);

};
}

#endif /* AURACHIP_HPP */
