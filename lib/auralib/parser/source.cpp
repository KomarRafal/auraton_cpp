/*
 * source.cpp
 */

#include "source.hpp"

namespace aura
{

namespace parser
{

std::optional<source_type> source::parse(std::string_view& message) {
	const auto source_token = message.substr(0, SOURCE_TOKEN.size());
	const auto is_token_ok = (source_token == SOURCE_TOKEN);
	if (!is_token_ok) {
		return std::nullopt;
	}
	message.remove_prefix(SOURCE_TOKEN.size());
	const auto end = message.find(EOL);
	if (end == std::string_view::npos) 	{
		return std::nullopt;
	}
	const auto source = message.substr(0, end);
	message.remove_prefix(end + EOL.size());
	for (auto source_pair : source_map) {
		if (source_pair.first == static_cast<std::string>(source)) {
			return source_pair.second;
		}
	}
	return std::nullopt;
}

}

}
