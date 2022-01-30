/*
 * clear.cpp
 */
#include "clear.hpp"

#include "specific_source.hpp"
#include "status.hpp"
#include "at.hpp"

namespace aura
{

namespace parser
{

const std::string clear::SOURCE_CLEAR_TOKEN = "CLEAR";

const std::string& clear::get_token() const
{
	return CLEAR_TOKEN;
}

bool clear::is_value_ok(const std::string_view& value) const
{
	if (value.empty()) {
		return false;
	}
	const auto value_str = static_cast<std::string>(value);
	const char *start = (static_cast<std::string>(value_str)).c_str();
	auto percent = std::strtoul(start, nullptr, 10);
	if ((percent < 1) || (percent > 100)) {
		return false;
	}
	if (value_str.length() != std::to_string(percent).length()) {
		return false;
	}
	return true;
}

/*
AT:START
SOURCE:CLEAR
STATUS:OK
PROGRESS:1
AT:STOP
*/
parser_if::parser_algorithm_t clear_builder::build()
{
	parser_if::parser_ptr at_parser = std::make_unique<at>();
	parser_if::parser_ptr source_parser = std::make_unique<specific_source>(clear::SOURCE_CLEAR_TOKEN);
	parser_if::parser_ptr status_parser = std::make_unique<status>();
	parser_if::parser_ptr clear_parser = std::make_unique<clear>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(at_parser));
	parse_algorithm.push_back(std::move(source_parser));
	parse_algorithm.push_back(std::move(status_parser));
	parse_algorithm.push_back(std::move(clear_parser));
	return parse_algorithm;
}

}

}
