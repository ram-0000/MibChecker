#include "ListParserContextExec.h"

ListParserContextExec::ListParserContextExec(ParserItem * entry)
{
	m_list.clear();
	FoundReset();

	// create first context
	ParserContextExec context;
	context.CallStackPush(entry);

	// add it to the list
	m_list.push_back(context);
}

ListParserContextExec::~ListParserContextExec(void)
{
	m_list.clear();
}

ParserContextExec & ListParserContextExec::Found(void)
{
	if(m_found.isSet() == false)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return m_found;
}

ParserContextExec & ListParserContextExec::Current(void)
{
	if(size() == 0)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return m_list.back();
}

void ListParserContextExec::CurrentRemove(void)
{
	if(size() == 0)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	m_list.pop_back();
}

void ListParserContextExec::CurrentDuplicate(ParserItem * item)
{
	ParserContextExec duplicate = Current();
	duplicate.PcSet(item);
	m_list.push_back(duplicate);
}

void ListParserContextExec::CurrentIntoFound(void)
{
	m_found = Current();
}

void ListParserContextExec::FoundIntoCurrent(void)
{
	if(IsFound() == false)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	m_list.clear();
	m_list.push_back(m_found);
	FoundReset();
}

void ListParserContextExec::FoundReset(void)
{
	m_found.Reset();
}

bool ListParserContextExec::IsFound(void) const
{
	return m_found.isSet();
}
