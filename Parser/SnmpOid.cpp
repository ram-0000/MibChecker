#include "SnmpOid.h"
#include "ParserException.h"
#include "Debug.h"

SnmpOid::SnmpOid(void)
{
	clear();
}

SnmpOid::~SnmpOid(void)
{
	clear();
}

void SnmpOid::clear(void)
{
	m_val.clear();
}

QString SnmpOid::toString(void) const
{
	QString str;
	for(const auto & item : m_val)
	{
		if(str.length() != 0)
			str += ".";

		if(item.isInteger() == true)
			str += QString::number(item.Integer());
		else
			str += "???";

	}
	return str;
}

void SnmpOid::Dump(int level, bool short_form) const
{
	QString indent(3 * level, ' ');
	QString str;

	if(short_form == true)
	{
		DEBUG("%s%s", indent.toLatin1().constData(), toString().toLatin1().constData());
		return;
	}

	for(const auto & item : m_val)
	{
		if(str.length() != 0)
			str += " . ";

		QString name = item.Name();
		if(name.length() != 0)
			str += name;

		if(item.isInteger() == true)
		{
			if(name.length() != 0) str += "(";
			str += QString::number(item.Integer());
			if(name.length() != 0) str += ")";
		}
		else if(item.isString() == true)
		{
			if(name.length() != 0) str += "(";
			str += item.String();
			if(name.length() != 0) str += ")";
		}
	}

	DEBUG("%s%s", indent.toLatin1().constData(), str.toLatin1().constData());
}

void SnmpOid::add(const QString & name)
{
	m_val.push_back(SnmpOidMember(name));
}

void SnmpOid::add(int val)
{
	m_val.push_back(SnmpOidMember(val));
}

void SnmpOid::set(const QString & val_identifier)
{
	if(m_val.size() == 0)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	m_val.back().Value(val_identifier);
}

void SnmpOid::set(int val_integer)
{
	if(m_val.size() == 0)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	m_val.back().Value(val_integer);
}

bool SnmpOid::isFullyDefined(void) const
{
	for(const auto & val : getVal())
	{
		if(val.isDefined() == false)
			return false;
	}
	return true;
}
