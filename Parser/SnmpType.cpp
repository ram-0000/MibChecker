#include "SnmpType.h"
#include "ParserException.h"
#include "Debug.h"

SnmpType::SnmpType(void)
{
	clear();
}

SnmpType::~SnmpType(void)
{
	clear();
}

void SnmpType::clear(void)
{
	m_name.clear();
	m_type = TypeNotSet;
	m_type_name.clear();
	m_type_extended.clear();
	m_compound.clear();
	m_mib_module.clear();
	m_mib_line = -1;
}

void SnmpType::Dump(int level) const
{
	QString indent(3 * level, ' ');
	QString str;

	// name if exist
	if(m_name.length() != 0)
	{
		str += Name();
		str+= ": ";
	}

	// type itself
	str += type_str(m_type);
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

	DEBUG("%s%s(%d) ==> %s",
			indent.toLatin1().constData(),
			MibModule().toLatin1().constData(),
			MibLine(),
			str.toLatin1().constData());

	// compound types
	for(int boucle = 0; boucle != m_compound.size(); boucle++)
		m_compound[boucle].Dump(level + 1);
}

void SnmpType::addCompound(SnmpType & type)
{
	m_compound.push_back(type);
	type.clear();
}

const char * SnmpType::type_str(BaseType_t type)
{
	if(type == TypeDefined) return "Defined";
	if(type == TypeNull) return "NULL";
	if(type == TypeBoolean) return "BOOLEAN";
	if(type == TypeReal) return "REAL";
	if(type == TypeInteger) return "INTEGER";
	if(type == TypeObjectIdentifier) return "OBJECT IDENTIFIER";
	if(type == TypeString) return "STRING";
	if(type == TypeBitString) return "BIT STRING";
	if(type == TypeBits) return "BITS";
	if(type == TypeSequence) return "SEQUENCE";
	if(type == TypeSequenceOf) return "SEQUENCE OF";
	if(type == TypeSet) return "SET";
	if(type == TypeSetOf) return "SET OF";
	if(type == TypeChoice) return "CHOICE";
	if(type == TypeEnumerated) return "ENUMERATED";
	if(type == TypeSelection) return "SELECTION";
	if(type == TypeAny) return "ANY";
	if(type == TypeMacroModuleIdentity) return "Macro Module-Identity";
	if(type == TypeMacroObjectIdentity) return "Macro Object-Identity";
	if(type == TypeMacroObjectType) return "Macro Object-Type";
	if(type == TypeMacroNotificationType) return "Macro Notification-Type";
	if(type == TypeMacroTrapType) return "Macro Trap-Type";
	if(type == TypeMacroTextualConvention) return "Macro Textual-Convention";
	if(type == TypeMacroObjectGroup) return "Macro Object-Group";
	if(type == TypeMacroNotificationGroup) return "Macro Notification-Group";
	if(type == TypeMacroModuleCompliance) return "Macro Module-Compliance";
	if(type == TypeMacroAgentCapabilities) return "Macro Agent-Capabilities";
	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
}

bool SnmpType::is_macro(BaseType_t type)
{
	if(type == TypeMacroModuleIdentity) return true;
	if(type == TypeMacroObjectIdentity) return true;
	if(type == TypeMacroObjectType) return true;
	if(type == TypeMacroNotificationType) return true;
	if(type == TypeMacroTrapType) return true;
	if(type == TypeMacroTextualConvention) return true;
	if(type == TypeMacroObjectGroup) return true;
	if(type == TypeMacroNotificationGroup) return true;
	if(type == TypeMacroModuleCompliance) return true;
	if(type == TypeMacroAgentCapabilities) return true;
	return false;
}

void SnmpType::setType(SnmpType & type)
{
	*this = type;
	type.clear();
}

SnmpType & SnmpType::addCompound(void)
{
	m_compound.push_back(SnmpType());
	return lastCompound();
}

SnmpType & SnmpType::lastCompound(void)
{
	if(m_compound.size() == 0)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return m_compound.back();
}
