#ifndef SNMPOIDMEMBER_H
#define SNMPOIDMEMBER_H

#include <QString>

class SnmpOidMember
{
public:
	SnmpOidMember(void);
	SnmpOidMember(const QString & name);
	SnmpOidMember(int val);

	void clear(void);

	void Name(const QString & name);
	inline const QString & Name(void) const { return m_name; }

	inline void Value(int val) { m_value_integer = val; m_is_integer = true; }
	inline void Value(const QString & val) { m_value_string = val; m_is_string = true; }

	inline bool isInteger(void) const { return m_is_integer; }
	inline int Integer(void) const { return m_value_integer; }

	inline bool isString(void) const { return m_is_string; }
	inline const QString & String(void) const { return m_value_string; }

	bool isDefined(void) const;

private:
	QString m_name;

	bool m_is_integer;
	int m_value_integer;

	bool m_is_string;
	QString m_value_string;

};

#endif // SNMPOIDMEMBER_H
