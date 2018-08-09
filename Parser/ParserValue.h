#ifndef PARSERVALUE_H
#define PARSERVALUE_H

#include <QString>

class ParserValue
{
public:
	ParserValue(void);
	~ParserValue(void);

	void set(const QString & val);
	void clear(void);
	const QString & get(void) const { return m_val; }

private:
	QString m_val;
};

#endif // PARSERVALUE_H
