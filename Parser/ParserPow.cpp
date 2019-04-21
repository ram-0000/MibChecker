#include "ParserPow.h"
#include "ParserGeneric.h"
#include "Debug.h"

ParserPow::ParserPow(ParserGeneric * This, const char * RuleName)
{
	// A NULL RuleName indicates root og the parser
	m_parser = This;
	m_order = 0;
	m_rule = RuleName;
	m_parser->RuleBegin(Rule());
}

ParserPow::~ParserPow()
{
	m_parser->RuleEnd(Rule());
}

