#ifndef SNMPSTATUS_H
#define SNMPSTATUS_H

#include <QString>
#include "StringMatch.h"

class SnmpStatus
{
public:
	typedef enum
	{	StatusNotset,
		StatusCurrent,
		StatusDeprecated,
		StatusObsolete,
		StatusMandatory,
		StatusOptional,
	} Status_t;

	SnmpStatus(Status_t status = StatusNotset);

	inline void Set(Status_t status) { m_status = status; }
	void Set(QString & str);
	inline Status_t Get(void) const { return m_status; }
	inline const char * StatusStr(void) const { return StringMatch::SnmpStatusTranslate(m_status); }

	void clear(void);

private:
	Status_t m_status;

};

#endif // SNMPSTATUS_H
