/*
 * next_device_id.hpp
 */

#ifndef NEXT_DEVICE_ID_HPP_
#define NEXT_DEVICE_ID_HPP_

#include <string_view>

#include "token_traverse.hpp"
#include "parser_if.hpp"

namespace aura
{

namespace parser
{

class next_device_id : public token_traverse
{
public:
	next_device_id();

private:
	static inline const std::string DEVICE_ID_TOKEN = "ID: ";
};

}

}

#endif
