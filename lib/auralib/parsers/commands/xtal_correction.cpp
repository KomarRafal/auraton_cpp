/*
 * xtal_correction.cpp
 */
#include <regex>

#include "xtal_correction.hpp"

#include "commands/specific_command.hpp"
#include "specific_source.hpp"
#include "status.hpp"
#include "at.hpp"

namespace aura
{

namespace parser
{

namespace commands
{

const std::string xtal_correction::COMMAND_XTAL_CORRECTION_TOKEN = "CRYSTALCORRECTION?";

const std::string xtal_correction::get_command_token()
{
	return COMMAND_XTAL_CORRECTION_TOKEN;
}

const std::string& xtal_correction::get_token() const
{
	return XTAL_CORRECTION_TOKEN;
}

bool xtal_correction::is_value_ok(const std::string_view& value) const
{
	const std::regex value_regex{"(\\+|-)?\\d+(\\s+)(\r?\n|$|$)"};
	const auto xtal_value = static_cast<std::string>(value);
	if (std::regex_match(xtal_value, std::regex(value_regex)))
	{
		return true;
	}
	return false;
}

parser_if::parser_algorithm_t xtal_correction_builder::build()
{
	parser_if::parser_ptr at_parser = std::make_unique<at>();
	parser_if::parser_ptr source_commnand_parser = std::make_unique<specific_source>(command::SOURCE_TOKEN);
	parser_if::parser_ptr test_command_parser = std::make_unique<specific_command>(xtal_correction::get_command_token());
	parser_if::parser_ptr status_parser = std::make_unique<status>();
	parser_if::parser_ptr xtal_correction_parser = std::make_unique<xtal_correction>();

	parser_if::parser_algorithm_t parse_algorithm;
	parse_algorithm.push_back(std::move(at_parser));
	parse_algorithm.push_back(std::move(source_commnand_parser));
	parse_algorithm.push_back(std::move(test_command_parser));
	parse_algorithm.push_back(std::move(status_parser));
	parse_algorithm.push_back(std::move(xtal_correction_parser));
	return parse_algorithm;
}

}

}

}
