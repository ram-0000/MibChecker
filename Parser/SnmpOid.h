#ifndef SNMPOID_H
#define SNMPOID_H

#include <QVector>
#include <QString>
#include "SnmpOidMember.h"

class SnmpOid
{
public:
	SnmpOid(void);
	~SnmpOid(void);

	void clear(void);

	void Dump(int level, bool short_form) const;

	void add(const QString & name);
	void add(int val);

	void set(const QString & val_identifier);
	void set(int val_integer);

	QString toString(void) const;

	inline const QVector<SnmpOidMember> & getVal(void) const { return m_val; }
	inline QVector<SnmpOidMember> & getVal(void) { return m_val; }
	bool isFullyDefined(void) const;

	bool isOidValue(void) const { return getVal().size() > 1; }

private:
	QVector<SnmpOidMember> m_val;
};

#endif // SNMPOID_H
