/*
 * version.hpp
 */

#ifndef COMMAND_VERSION_HPP_
#define COMMAND_VERSION_HPP_

#include "simple_token.hpp"

/*
PCODE: 300c
FVER: 2.4
HVER: 1.0
MANCODE: 30
 */

namespace aura
{

namespace parser
{

class version_pcode : public simple_token
{
public:
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string PCODE_TOKEN = "PCODE: ";
};

class version_fw_version : public simple_token
{
public:
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string FW_VERSION_TOKEN = "FVER: ";
};

class version_hw_version : public simple_token
{
public:
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string HW_VERSION_TOKEN = "HVER: ";
};

class version_man_code : public simple_token
{
public:
	virtual const std::string& get_token() const;
	virtual bool is_value_ok(const std::string_view& value) const;

private:
	const std::string MAN_CODE_TOKEN = "MANCODE: ";
};

}

}

#endif
