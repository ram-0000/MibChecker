#include "SnmpValue.h"
#include "Debug.h"

SnmpValue::SnmpValue(void)
{
	clear();
}

SnmpValue::~SnmpValue(void)
{
	clear();
}

void SnmpValue::clear(void)
{
	m_name.clear();
	m_mib_module.clear();
	m_mib_line = -1;

	m_type.clear();

	m_oid.clear();
	m_access.clear();
	m_status.clear();
	m_description.clear();
}

void SnmpValue::Dump(int level) const
{
	QString indent(3 * level, ' ');
	DEBUG("%s%s(%d) ==> %s",
			indent.toLatin1().constData(),
			MibModule().toLatin1().constData(),
			MibLine(),
			getName().toLatin1().constData());

	m_type.Dump(level + 1);
	m_oid.Dump(level + 1, true);
}
