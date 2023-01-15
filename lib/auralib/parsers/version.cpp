/*
 * verion.cpp
 */

#include "version.hpp"

namespace aura
{

namespace parser
{

const std::string& version_pcode::get_token() const
{
	return PCODE_TOKEN;
}

bool version_pcode::is_value_ok(const std::string_view& value) const
{
	return (value.size() != 0);
}

const std::string& version_fw_version::get_token() const
{
	return FW_VERSION_TOKEN;
}

bool version_fw_version::is_value_ok(const std::string_view& value) const
{
	return (value.size() != 0);
}

const std::string& version_hw_version::get_token() const
{
	return HW_VERSION_TOKEN;
}

bool version_hw_version::is_value_ok(const std::string_view& value) const
{
	return (value.size() != 0);
}

const std::string& version_man_code::get_token() const
{
	return MAN_CODE_TOKEN;
}

bool version_man_code::is_value_ok(const std::string_view& value) const
{
	return (value.size() != 0);
}

}

}
