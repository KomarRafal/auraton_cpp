/*
 * device_parameter.cpp
 */

#include "device_parameter.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

parser_if::parsed_value device_parameter::parse(std::string_view& message) {
	const auto token = message.substr(0, DEVICE_PARAMETER_TOKEN.size());
	const auto is_token_ok = (token == DEVICE_PARAMETER_TOKEN);
	if (!is_token_ok) {
		return std::nullopt;
	}
	// omit first token and find next or EOF
	auto next_device = message.find(DEVICE_PARAMETER_TOKEN, DEVICE_PARAMETER_TOKEN.size());

	auto device_str = message.substr(0, next_device);
	if (next_device == std::string::npos) {
		// remove everything
		message.remove_prefix(message.size());
	}
	else {
		message.remove_prefix(next_device);
	}
	return static_cast<std::string>(device_str);
}

}

}

}
