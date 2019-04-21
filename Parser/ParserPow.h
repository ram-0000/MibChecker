#ifndef PARSERPOW_H
#define PARSERPOW_H

#include <QString>
#include <QVector>
#include "ParserException.h"

class ParserGeneric;

class ParserPow
{
public:
	ParserPow(ParserGeneric * This, const char * RuleName);
	~ParserPow(void);

	inline const char * Rule(void) const { return m_rule; }
	inline int Order(void) { return ++m_order; }

private:
	ParserGeneric * m_parser;
	const char * m_rule;
	int m_order;
};

#endif // PARSERPOW_H
