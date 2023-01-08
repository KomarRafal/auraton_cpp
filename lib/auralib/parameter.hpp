/*
 * parameter.hpp
 */

#ifndef PARAMETER_HPP_
#define PARAMETER_HPP_

#include <optional>
#include <cstdint>

namespace aura
{

class parameter {
public:
	parameter() = delete;
	bool operator==(parameter const& param) const;
	explicit parameter(int32_t code,
				  	   int32_t channel = 0,
					   int32_t flag_own = 0,
					   int32_t flag_writable = 0,
					   int32_t value = 0);

	int32_t get_code() const;
	int32_t get_channel() const;
	int32_t get_flag_own() const;
	int32_t get_flag_writable() const;
	int32_t get_value() const;
	void set_channel(int32_t value);
	void set_flag_own(int32_t value);
	void set_flag_writable(int32_t value);
	void set_value(int32_t new_value);
	void clear();

	static std::optional<parameter> get_next_parameter(std::string_view& message);
	static const uint16_t MAX_PARAM_LENGTH = 80;
private:
	int32_t code;
	int32_t channel;
	int32_t flag_own;
	int32_t writable;
	int32_t value;

};

}

#endif
