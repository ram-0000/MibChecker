#include "ParserValue.h"

ParserValue::ParserValue(void)
{
	clear();
}

ParserValue::~ParserValue(void)
{
	clear();
}

void ParserValue::clear(void)
{
	m_val.clear();
}

void ParserValue::set(const QString & val)
{
	m_val = val;
}

