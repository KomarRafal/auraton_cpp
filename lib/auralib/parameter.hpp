/*
 * parameter.hpp
 */

#ifndef PARAMETER_HPP_
#define PARAMETER_HPP_

#include <cstdint>

namespace aura
{

class parameter {
public:
	parameter() = delete;
	bool operator==(parameter const& param) const {
		return (
				(param.code == code) &&
				(param.channel == channel) &&
				(param.flag_own == flag_own) &&
				(param.writable == writable) &&
				(param.value == value)
				);
	}

	explicit parameter(uint32_t code,
			  int32_t channel = 0,
			  int32_t flag_own = 0,
			  int32_t flag_writable = 0,
			  int32_t value = 0) :
			code(code),
			channel(channel),
			flag_own(flag_own),
			writable(flag_writable),
			value(value)
		{ }

	int32_t get_code() const {
		return code;
	}

	int32_t get_channel() const {
		return channel;
	}

	int32_t get_flag_own() const {
		return flag_own;
	}

	int32_t get_flag_writable() const {
		return writable;
	}

	int32_t get_value() const {
		return value;
	}

	void set_channel(int32_t value) {
		channel = value;
	}

	void set_flag_own(int32_t value) {
		flag_own = value;
	}

	void set_flag_writable(int32_t value) {
		writable = value;
	}

	void set_value(int32_t new_value) {
		value = new_value;
	}
	void clear() {
		code = 0;
		channel = 0;
		flag_own = 0;
		writable = 0;
		value = 0;
	}

	static const uint16_t MAX_BYTES = 80;
private:
	uint32_t code;
	int32_t channel;
	int32_t flag_own;
	int32_t writable;
	int32_t value;

};

}

#endif /* PARAMETER_HPP_ */
