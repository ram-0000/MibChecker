#include "SnmpStatus.h"
#include "StringMatch.h"

SnmpStatus::SnmpStatus(Status_t status /*= StatusNotset*/)
{
	clear();
	Set(status);
}

void SnmpStatus::clear(void)
{
	m_status = StatusNotset;
}

void SnmpStatus::Set(QString & str)
{
	int status = StringMatch::SnmpStatusTranslate(str.toLatin1().constData());
	Set(static_cast<Status_t>(status));
	str.clear();
}
