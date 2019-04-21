#ifndef SNMPACCESS_H
#define SNMPACCESS_H

#include <QString>
#include "StringMatch.h"

class SnmpAccess
{
public:
	typedef enum
	{	AccessNotset,
		AccessNotAccessible,
		AccessAccessibleForNotify,
		AccessReadOnly,
		AccessReadWrite,
		AccessReadCreate,
		AccessNotImplemented,
		AccessWriteOnly,
	} Access_t;

	SnmpAccess(Access_t Access = AccessNotset);

	inline void Set(Access_t access) { m_access = access; }
	void Set(QString & str);
	inline Access_t Get(void) const { return m_access; }
	inline const char * AccessStr(void) const { return StringMatch::SnmpAccessTranslate(m_access); }

	void clear(void);

private:
	Access_t m_access;

};

#endif // SNMPACCESS_H
