/*
 * parser.cpp
 */
#include <functional>
#include <limits>

#include "parser.hpp"

namespace aura
{

const std::string parser::OK_TOKEN = "OK";
const std::string parser::DEVICE_TOKEN = "ID: ";
const std::string parser::EOL = "\r\n";
const std::string parser::CODE_TOKEN = "CODE: ";
const std::string parser::CHANNEL_TOKEN = "CHANNEL: ";
const std::string parser::FLAG_OWN_TOKEN = "FLAG OWN: ";
const std::string parser::FLAG_WRITEABLE_TOKEN = "FLAG WRITEABLE: ";
const std::string parser::VALUE_TOKEN = "VALUE: ";


std::string parser::parse(const std::string& input_str, const std::string& token, bool omit_token) {
	auto begin = input_str.find(token);
	if (begin == std::string::npos) {
		return {};
	}
	if (omit_token) {
		begin += token.length();
	}
	const auto end = input_str.find(EOL, begin);
	return input_str.substr(begin, end - begin);
}

parser::device_list_t parser::parse_device_list(const std::string& input_str) {
	device_list_t device_list;
	size_t begin = 0;

	while (true)
	{
		size_t end;
		begin = input_str.find(DEVICE_TOKEN, begin);
		if (begin == std::string::npos) {
			return device_list;
		}
		end = input_str.find(EOL, begin);
		begin += DEVICE_TOKEN.length();
		auto dev_id_str = input_str.substr(begin, end - begin);
		if (dev_id_str.empty()) {
			return device_list;
		}
		auto dev_id = std::stoul(dev_id_str);
		begin = end + EOL.length();
		end = input_str.find(DEVICE_TOKEN, begin);
		if (device_list.find(dev_id) == device_list.end()) {
			device_list[dev_id] = input_str.substr(begin, end - begin);
		}
		else {
			device_list.find(dev_id)->second.append(input_str.substr(begin, end - begin));
		}
		begin = end;
	}

}

bool parser::get_next_parameter(std::string& input_str, parameter& read_parameter) {
	std::map<std::string, std::function<void(parameter&, int32_t)>> parameter_info = {
			{ parser::CHANNEL_TOKEN, [](auto& param, auto value) { param.set_channel(value); }  },
			{ parser::FLAG_OWN_TOKEN, [](auto& param, auto value) { param.set_flag_own(value); }  },
			{ parser::FLAG_WRITEABLE_TOKEN, [](auto& param, auto value) { param.set_flag_writable(value); }  },
			{ parser::VALUE_TOKEN, [](auto& param, auto value) { param.set_value(value); }  },
	};

	while (true) {
		auto begin = input_str.find(CODE_TOKEN);
		if (begin == std::string::npos) {
			return false;
		}
		auto is_code_token_corrupted = ( (begin > 0) && (input_str[begin - 1] != '\n') );
		const auto code_end = input_str.find(EOL, begin);
		begin += CODE_TOKEN.length();
		const auto dev_code_str = input_str.substr(begin, code_end - begin);
		is_code_token_corrupted |= dev_code_str.empty();
		if (is_code_token_corrupted) {
			input_str.erase(0, code_end);
			continue;
		}
		const auto code_long = std::stoul(dev_code_str);
		if (code_long > std::numeric_limits<std::uint32_t>::max()) {
			input_str.erase(0, code_end);
			continue;
		}
		const auto code = static_cast<uint32_t>(code_long);
		parameter local_param{code};
		const auto next_parameter = input_str.find(CODE_TOKEN, code_end);
		const auto parameter_str = input_str.substr(code_end, next_parameter);
		for (const auto& info : parameter_info) {
			const auto value_str = parser::parse(parameter_str, info.first);
			if (value_str.empty()) {
				continue;
			}
			const auto value = std::stoul(value_str);
			info.second(local_param, value);
		}
		input_str.erase(0, next_parameter);
		read_parameter = local_param;
		return true;
	}
}

bool parser::check_result(const std::string& input_str) {
	const std::string status = parse(input_str, OK_TOKEN, false);
	return (status.length() == OK_TOKEN.length());
}

}
