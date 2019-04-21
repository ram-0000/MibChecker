#include "SnmpType.h"
#include "ParserException.h"
#include "Debug.h"
#include "StringMatch.h"

SnmpType::SnmpType(void)
{
	m_lock = false;
	clear();
}

void SnmpType::clear(void)
{
	if(isLocked() == true)
		return;

	m_name.clear();
	m_type = TypeNotSet;
	m_type_name.clear();
	m_type_extended.clear();
	m_compound.clear();
	m_mib_module.clear();
	m_mib_line = -1;
	m_user_choice = false;
	Description("", true);
}

void SnmpType::Name(QString & val)
{
	if(isLocked() == true)
		return;

	m_name = val;
	val.clear();
}

void SnmpType::MibModule(const QString & val)
{
	if(isLocked() == true)
		return;

	m_mib_module = val;
}

void SnmpType::MibLine(int val)
{
	if(isLocked() == true)
		return;

	m_mib_line = val;
}

void SnmpType::Description(const QString & str, bool override /*= false*/)
{
	if(isLocked() == true)
		return;

	if( (override == true) || (Description().length() == 0) )
		m_description = str;
}

void SnmpType::setType(SnmpType & type)
{
	if(isLocked() == true)
		return;

	//	DEBUG("Setting type");
	*this = type;
	type.clear();
}

void SnmpType::UserChoice(bool user_choice)
{
	if(isLocked() == true)
		return;

	m_user_choice = user_choice;
}

void SnmpType::addTypeExtended(const QString & str)
{
	if(isLocked() == true)
		return;

	//DEBUG("Adding extended");
	m_type_extended += str;
}

SnmpType & SnmpType::addCompound(void)
{
	if(isLocked() == true)
		return *this;

	//DEBUG("Adding compound");
	m_compound.push_back(SnmpType());
	return lastCompound();
}

SnmpType & SnmpType::lastCompound(void)
{
	if(m_compound.size() == 0)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return m_compound.back();
}

void SnmpType::set(BaseType_t type, const QString & name /*= ""*/)
{
	if(isLocked() == true)
		return ;
	m_type = type;
	m_type_name = name;
}


void SnmpType::Dump(int level) const
{
	QString indent(3 * level, ' ');
	QString str;

	// name if exist
	if(Name().length() != 0)
	{
		str += Name();
		str+= ": ";
	}

	// type itself
	str += StringMatch::SnmpTypeTranslate(m_type);
	if(m_type_name.length() != 0)
	{
		str += "=";
		str += m_type_name;
	}

	// extension is exist
	if(m_type_extended.length() != 0)
	{
		str += ": ";
		str += m_type_extended;
	}

	// Description if exist
	if(Description().length() != 0)
	{
		str += " Descr='";
		str += Description();
		str+= "'";
	}

	DEBUG("%s%s(%d) ==> %s",
			indent.toLatin1().constData(),
			MibModule().toLatin1().constData(),
			MibLine(),
			str.toLatin1().constData());

	// compound types
	for(int boucle = 0; boucle != m_compound.size(); boucle++)
		m_compound[boucle].Dump(level + 1);
}

