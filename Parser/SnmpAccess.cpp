#include "SnmpAccess.h"
#include "StringMatch.h"

SnmpAccess::SnmpAccess(Access_t access /*= AccessNotset*/)
{
	clear();
	Set(access);
}

void SnmpAccess::clear(void)
{
	m_access = AccessNotset;
}

void SnmpAccess::Set(QString & str)
{
	int Access = StringMatch::SnmpAccessTranslate(str.toLatin1().constData());
	Set(static_cast<Access_t>(Access));
	str.clear();
}
