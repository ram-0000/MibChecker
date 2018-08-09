#ifndef SNMPVALUE_H
#define SNMPVALUE_H

#include <QString>
#include "SnmpOid.h"
#include "SnmpType.h"

class SnmpValue
{
public:
	SnmpValue(void);
	~SnmpValue(void);

	void clear(void);

	void Dump(int level) const;

	inline void UserChoice(bool user_choice) { m_user_choice = user_choice; }
	inline bool isUserChoice(void) const { return m_user_choice; }

	inline void setName(const QString & val) { m_name = val; }
	inline void Name(QString & val) { m_name = val; val.clear(); }
	inline const QString & getName(void) const { return m_name; }

	inline void setAccess(const QString & str) { m_access = str; }
	inline void setStatus(const QString & str) { m_status = str; }
	inline void setDescription(const QString & str) { m_description = str; }
	inline const QString & getDescription(void) const { return m_description; }

	const QString & MibModule(void) const { return m_mib_module; }
	void MibModule(const QString & val) { m_mib_module = val; }

	int MibLine(void) const { return m_mib_line; }
	void MibLine(int val) { m_mib_line = val; }

	SnmpOid m_oid;
	SnmpType m_type;

private:
	QString m_mib_module;
	int m_mib_line;

	QString m_name;
	bool m_user_choice;

	QString m_access;
	QString m_status;
	QString m_description;
};

#endif // SNMPVALUE_H
