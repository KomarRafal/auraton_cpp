/*
 * timeout.hpp
 */

#ifndef TIMEOUT_HPP_
#define TIMEOUT_HPP_

#include "gmock/gmock.h"
#include "gtest/gtest.h"

class TimeoutInstance {
public:
	TimeoutInstance() = default;
	virtual ~TimeoutInstance() = default;
	MOCK_METHOD(void, sleep_for_ms, (uint32_t duration));
};

class Timeout {
public:
	Timeout() = default;
	virtual ~Timeout() {
		clear();
	}

    static void sleep_for_ms(uint32_t duration) {
    	ASSERT_NE(instance, nullptr);
    	if (instance != nullptr) {
    		instance->sleep_for_ms(duration);
    	}
    }

    static auto& get_instance() {
    	if (instance == nullptr) {
    		instance = new TimeoutInstance;
    	}
    	return *instance;
    }

    static void clear() {
    	if (instance != nullptr) {
    		delete instance;
    		instance = nullptr;
    	}
    }

private:
    inline static TimeoutInstance* instance = nullptr;
};

class TimeoutRAII {
public:
	TimeoutRAII() = default;
	~TimeoutRAII() {
		Timeout::clear();
	}

	static auto& get_instance() {
		return Timeout::get_instance();
	}
};

#endif /* TIMEOUT_HPP_ */
