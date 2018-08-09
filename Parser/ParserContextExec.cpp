#include "ParserContextExec.h"

ParserContextExec::ParserContextExec(void)
{
	Reset();
}

ParserContextExec::~ParserContextExec(void)
{
	Reset();
}

void ParserContextExec::Reset(void)
{
	m_callstack.clear();
	m_callback_delayed.clear();
	m_pc = NULL;
}

bool ParserContextExec::isSet(void) const
{
	if(m_pc != NULL)
		return true;
	if(m_callstack.size() != 0)
		return true;
	return false;
}

void ParserContextExec::CallStackPush(ParserItem * item)
{
	ParserException::CheckNull(item);

	// do no use PcGet() in this place because sometimes (when initializing)
	// PC is NULL and PcGet() will throw exception
	m_callstack.push_back(m_pc);
	PcSet(item);
}

void ParserContextExec::CallStackPop(void)
{
	if(m_callstack.size() == 0)
		throw ParserExceptionPopEmptyStack();
	m_pc = m_callstack.back();
	m_callstack.pop_back();

	// when PC becomes NULL, we probably reach end of parsing
	if(m_pc != NULL)
		PcNext();
}

QString ParserContextExec::CallStackStr(void) const
{
	QString ret = "";
	for(ParserItemList::const_iterator it = m_callstack.begin(); it != m_callstack.end(); ++it)
	{
		if(ret.length() != 0)
			ret += "/";
		ret += (*it)->Rule();
	}
	return ret;
}

void ParserContextExec::PcSet(ParserItem * item)
{
	ParserException::CheckNull(item);
	m_pc = item;
}

ParserItem * ParserContextExec::PcGet(void) const
{
	ParserException::CheckNull(m_pc);
	return m_pc;
}

void ParserContextExec::PcNext(void)
{
	ParserException::CheckNull(m_pc);
	m_pc = m_pc->Next();
	ParserException::CheckNull(m_pc);
}
