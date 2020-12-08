/*
 * timeout.hpp
 */

#ifndef TIMEOUT_HPP_
#define TIMEOUT_HPP_

#include <memory>
#include <chrono>
#include <thread>

class Timeout {
public:
    static auto sleep_for_ms(uint32_t duration) {
    	return std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int64_t>(duration)));
    }
};

#endif /* TIMEOUT_HPP_ */
