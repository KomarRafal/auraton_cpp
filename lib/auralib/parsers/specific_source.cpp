/*
 * specific_source.cpp
 */

#include "specific_source.hpp"

namespace aura
{

namespace parser
{

const std::string specific_source::COMMAND = "COMMAND";
const std::string specific_source::EVENT = "EVENT";

specific_source::specific_source(std::string specific_source) :
	source(specific_source) {
}

bool specific_source::is_value_ok(const std::string_view& value) const {
	return value == source;
}

}

}
