/*
 * source.cpp
 */

#include "source.hpp"

namespace aura
{

namespace parser
{

const std::string& source::get_token() const
{
	return SOURCE_TOKEN;
}

const source::token_map& source::get_token_map() const
{
	return source_map;
}

}

}
