#include "ParserItem.h"
#include "Debug.h"

ParserItem::ParserItem(const char * rule, ParserItemType_t type)
{
	// record item type
	m_id = -1;
	m_next = nullptr;
	m_type = type;

	// record rule name
	ParserException::CheckNullOrEmpty(rule);
	m_rule = rule;

	m_value_number = -1;
	m_value_string = nullptr;

	m_jump_table.clear();

	m_callback_fct = nullptr;
	m_callback_order = -1;

}

ParserItem::~ParserItem(void)
{
	// do not delete object pointed by table
	m_jump_table.clear();
}

ParserItem * ParserItem::Next(void) const
{
	ParserException::CheckNull(m_next);
	return m_next;
}

void ParserItem::Finalize(int id, ParserItem * next)
{
	if(m_id != -1)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	m_id = id;

	if(m_next != nullptr)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	m_next = next;
}

int ParserItem::Id(void) const
{
	if(m_id == -1)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return m_id;
}

void ParserItem::Token(int token)
{
	if( (isToken() == false) && (isGreedy() == false) )
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	m_value_number = token;
}

int ParserItem::Token(void) const
{
	if( (isToken() == false) && (isGreedy() == false) )
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	if(m_value_number == -1)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	return m_value_number;
}

void ParserItem::Sub(const char * sub)
{
	if(isSub() == false)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	ParserException::CheckNullOrEmpty(sub);
	m_value_string = sub;
}

const char * ParserItem::Sub(void) const
{
	if(isSub() == false)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
	ParserException::CheckNullOrEmpty(m_value_string);
	return m_value_string;
}

void ParserItem::Jump(ParserItem * jump)
{
	// only branch items can be jumpers
	if(isJump() == false)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	ParserException::CheckNull(jump);
	m_jump_table.push_back(jump);
}

const ParserItemList & ParserItem::Jump(void) const
{
	// only start and branch items can be jumpers
	if(isJump() == false)
		throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);

	return m_jump_table;
}

const char * ParserItem::TypeStr(void) const
{
	switch(Type())
	{
	case TypeToken: return "Token";
	case TypeGreedy: return "Greedy";
	case TypeJump: return "Jump";
	case TypePoint: return "Point";
	case TypeBegin: return "Begin";
	case TypeEnd: return "End";
	case TypeSub: return "Sub";
	case TypeCallback: return "Callback";
	}
	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
}

void ParserItem::Dump(void) const
{
	// build header text
	QString h = QString::asprintf("Item %d/%s ==> %s", Id(), Rule(), TypeStr());
	h.toLatin1().constData();

	if(isToken() == true)
	{
		DEBUG("%s %d", h.toLatin1().constData(), Token());
		return;
	}

	if(isGreedy() == true)
	{
		DEBUG("%s %d", h.toLatin1().constData(), Token());
		return;
	}

	if(isBegin() == true)
	{
		DEBUG("%s", h.toLatin1().constData());
		return;
	}

	if(isEnd() == true)
	{
		DEBUG("%s", h.toLatin1().constData());
		return;
	}

	if(isSub() == true)
	{
		DEBUG("%s %s", h.toLatin1().constData(), Sub());
		return;
	}

	if(isJump() == true)
	{
		// list of branch table
		QString j_str;
		const ParserItemList & j = Jump();
		for(ParserItemList::const_iterator it = j.begin(); it != j.end(); ++it)
		{
			if(j_str.length() != 0) j_str += ", ";
			j_str += QString::number((*it)->Id());
		}
		DEBUG("%s %s", h.toLatin1().constData(), j_str.toLatin1().constData());
		return;
	}

	if(isPoint() == true)
	{
		DEBUG("%s", h.toLatin1().constData());
		return;
	}

	if(isCallback() == true)
	{
		DEBUG("%s", h.toLatin1().constData());
		return;
	}

	throw ParserExceptionShouldNotArrive(__FILE__, __LINE__);
}

void ParserItem::Callback(ParserAction * callback, int Order)
{
	ParserException::CheckNull(callback);
	m_callback_fct = callback;
	m_callback_order = Order;
}
