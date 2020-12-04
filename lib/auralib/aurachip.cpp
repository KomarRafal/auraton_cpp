/*
 * aurachip.cpp
 */
#include "aurachip.hpp"
#include "parser.hpp"

namespace aura
{

std::map<const uint32_t, const std::string> cmd::command_map = {
		{ cmd::TEST, "TEST?" },
		{ cmd::VERSION, "VER?" },
		{ cmd::LINK, "LINK" },
		{ cmd::EVENT_LINK, "EVENT_LINK" },
		{ cmd::ADDRESS, "ADDR?" },
		{ cmd::DEV_LIST, "LIST?" },
		{ cmd::RESET, "RST" },
		{ cmd::LAST_CMD, "" }
};

const std::string cmd::compose(const cmd::code& command) {
	std::string composed_command("AT+");
	composed_command.append(cmd::Get(command));
	composed_command.append("\n");
	return composed_command;
}

bool chip::test() {
	return simple_command(cmd::compose(cmd::TEST));
}

void chip::initialize() {
	auto device_str = send_command(cmd::compose(cmd::VERSION));
	device_str.append(send_command(cmd::compose(cmd::ADDRESS)));
	device dev(device_str);
	static_cast<device*>(this)->operator=(dev);
	initialize_flag = true;
}

bool chip::link() {
	const bool is_cmd_ok = simple_command(cmd::compose(cmd::LINK));
	if (!is_cmd_ok)
		return false;
	const bool is_event = wait_for_read(LINK_WAIT_MS);
	if (!is_event)
		return false;
	const bool is_event_ok = check_event(cmd::Get(cmd::EVENT_LINK));
	return is_event_ok;
}

bool chip::reset() {
	return simple_command(cmd::compose(cmd::RESET));
}

int chip::update_device_list() {
	const auto dev_list_str = send_command(cmd::compose(cmd::DEV_LIST));
	const auto dev_list_parsed = parser::parse_device_list(dev_list_str);
	device_list.clear();
	for (auto dev_pair : dev_list_parsed) {
		const device dev(dev_pair.second);
		device_list[dev_pair.first] = dev;
	}
	return device_list.size();
}

}
