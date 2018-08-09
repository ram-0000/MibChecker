#include "SnmpOidMember.h"

SnmpOidMember::SnmpOidMember(void)
{
	clear();
}

SnmpOidMember::SnmpOidMember(const QString & name)
{
	clear();
	Name(name);
}

SnmpOidMember::SnmpOidMember(int val)
{
	clear();
	Value(val);
}

void SnmpOidMember::clear(void)
{
	Name("");
	Value(-1);
	Value("");
	m_is_integer = false;
	m_is_string = false;
}

void SnmpOidMember::Name(const QString & name)
{
	m_name = name;

	// set default value for well known names
	if(Name() == "iso")
		Value(1);
}

bool SnmpOidMember::isDefined(void) const
{
	return isInteger();
}
