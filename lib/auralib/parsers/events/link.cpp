/*
 * link.cpp
 */

#include "parser_executor.hpp"
#include "specific_source.hpp"
#include "specific_event.hpp"
#include "direction.hpp"
#include "status.hpp"
#include "link.hpp"
#include "at.hpp"

namespace aura
{

namespace parser
{

namespace events
{

parser_if::parsed_value link::parse(std::string_view& message)
{
	// TODO: const everywhere
	parser_if::parser_ptr direction_parser = std::make_unique<direction::receive>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(direction_parser));

	const auto is_test_ok = parser::parser_executor::execute(
			message,
			parse_algorithm);
	return is_test_ok;
}

const std::string link::EVENT_LINK_TOKEN = "LINK";

const std::string link::get_event_token()
{
	return EVENT_LINK_TOKEN;
}

parser_if::parser_algorithm_t link_builder::build()
{
	parser_if::parser_ptr at_parser = std::make_unique<at>();
	parser_if::parser_ptr source_commnand_parser = std::make_unique<specific_source>(event::SOURCE_TOKEN);
	parser_if::parser_ptr link_event_parser = std::make_unique<specific_event>(link::get_event_token());
	parser_if::parser_ptr status_parser = std::make_unique<status>();
	parser_if::parser_ptr link_parser = std::make_unique<link>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(at_parser));
	parse_algorithm.push_back(std::move(source_commnand_parser));
	parse_algorithm.push_back(std::move(link_event_parser));
	parse_algorithm.push_back(std::move(status_parser));
	parse_algorithm.push_back(std::move(link_parser));
	return parse_algorithm;
}

}

}

}
