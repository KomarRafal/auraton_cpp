/*
 * parameter.cpp
 */
#include <string_view>
#include <functional>
#include <utility>
#include <vector>

#include "parsers/next_parameter.hpp"
#include "parsers/simple_numeric.hpp"
#include "parameter.hpp"

namespace aura
{

bool parameter::operator==(parameter const& param) const {
	return (
			(param.code == code) &&
			(param.channel == channel) &&
			(param.flag_own == flag_own) &&
			(param.writable == writable) &&
			(param.value == value)
			);
}

parameter::parameter(int32_t code,
					 int32_t channel,
					 int32_t flag_own,
					 int32_t flag_writable,
					 int32_t value) :
		code(code),
		channel(channel),
		flag_own(flag_own),
		writable(flag_writable),
		value(value)
	{ }

int32_t parameter::get_code() const {
	return code;
}

int32_t parameter::get_channel() const {
	return channel;
}

int32_t parameter::get_flag_own() const {
	return flag_own;
}

int32_t parameter::get_flag_writable() const {
	return writable;
}

int32_t parameter::get_value() const {
	return value;
}

void parameter::set_channel(int32_t value) {
	channel = value;
}

void parameter::set_flag_own(int32_t value) {
	flag_own = value;
}

void parameter::set_flag_writable(int32_t value) {
	writable = value;
}

void parameter::set_value(int32_t new_value) {
	value = new_value;
}
void parameter::clear() {
	code = 0;
	channel = 0;
	flag_own = 0;
	writable = 0;
	value = 0;
}

std::optional<parameter> parameter::get_next_parameter(std::string_view& message) {
	const std::string CHANNEL_TOKEN = "CHANNEL: ";
	const std::string FLAG_OWN_TOKEN = "FLAG OWN: ";
	const std::string FLAG_WRITEABLE_TOKEN = "FLAG WRITEABLE: ";
	const std::string VALUE_TOKEN = "VALUE: ";

	parser::next_parameter parameter_parser;

	for (auto parameter_str = parameter_parser.parse(message);
		 parameter_str.has_value();
		 parameter_str = parameter_parser.parse(message)) {
		std::string_view parameter_string_view{*parameter_str};
		aura::parser::simple_numeric code_parser{parameter_parser.get_token()};
		auto code_str = code_parser.parse(parameter_string_view);
		if (!code_str.has_value()) {
			continue;
		}
		const auto code = std::stoi(*code_str);
		parameter local_param{code};

		std::vector<
			std::pair<parser::simple_numeric, std::function<void(parameter&, int32_t)>>
		> parameter_info = {
				{ parser::simple_numeric{CHANNEL_TOKEN}, [](auto& param, auto value) { param.set_channel(value); }  },
				{ parser::simple_numeric{FLAG_OWN_TOKEN}, [](auto& param, auto value) { param.set_flag_own(value); }  },
				{ parser::simple_numeric{FLAG_WRITEABLE_TOKEN}, [](auto& param, auto value) { param.set_flag_writable(value); }  },
				{ parser::simple_numeric{VALUE_TOKEN}, [](auto& param, auto value) { param.set_value(value); }  },
		};
		bool parameter_ok = true;
		for (auto& [parser, setter] : parameter_info) {
			const auto value_str = parser.parse(parameter_string_view);
			if (!value_str.has_value()) {
				parameter_ok = false;
				break;
			}
			const auto value = std::stoi(*value_str);
			setter(local_param, value);
		}
		if (!parameter_ok) {
			continue;
		}
		return local_param;
	}

	return {};
}

}

