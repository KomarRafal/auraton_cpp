/*
 * timeout.hpp
 */

#ifndef TIMEOUT_HPP_
#define TIMEOUT_HPP_

#include "gmock/gmock.h"

class TimeoutInstance {
public:
	TimeoutInstance() = default;
	~TimeoutInstance() = default;
	MOCK_METHOD(void, sleep_for_ms, (uint32_t duration));
//	int sleep_for_ms([[maybe_unused]]uint32_t duration) {
//		return 0;
//	}
};

class Timeout {
public:
    static void sleep_for_ms(uint32_t duration) {
    	instance.sleep_for_ms(duration);
    }

    static auto& get_instance() {
    	return instance;
    }

private:
    inline static TimeoutInstance instance;
};

#endif /* TIMEOUT_HPP_ */
