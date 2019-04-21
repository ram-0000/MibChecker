#ifndef SNMPVALUE_H
#define SNMPVALUE_H

#include <QString>
#include "SnmpOid.h"
#include "SnmpType.h"
#include "SnmpStatus.h"
#include "SnmpAccess.h"

class SnmpValue
{
public:
	SnmpValue(void);
	~SnmpValue(void);

	void clear(void);

	void Dump(int level) const;

	inline void UserChoice(bool user_choice) { m_user_choice = user_choice; }
	inline bool isUserChoice(void) const { return m_user_choice; }

	inline void NameAndClear(QString & val) { m_name = val; val.clear(); }
	inline const QString & Name(void) const { return m_name; }

	const QString & MibModule(void) const { return m_mib_module; }
	void MibModule(const QString & val) { m_mib_module = val; }

	int MibLine(void) const { return m_mib_line; }
	void MibLine(int val) { m_mib_line = val; }

	SnmpOid m_oid;
	SnmpType m_type;
	SnmpStatus m_status;
	SnmpAccess m_access;

private:
	QString m_mib_module;
	int m_mib_line;

	bool m_user_choice;
	QString m_name;

};

#endif // SNMPVALUE_H
