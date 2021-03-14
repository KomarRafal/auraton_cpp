/*
 * status.cpp
 */

#include "status.hpp"

namespace aura
{

namespace parser
{

const std::string& status::get_token() const
{
	return STATUS_TOKEN;
}

const status::token_map& status::get_token_map() const
{
	return status_map;
}

}

}
