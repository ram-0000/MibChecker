#ifndef PARSERITEM_H
#define PARSERITEM_H

#include <QLinkedList>
#include <QMap>
#include "ParserException.h"
#include "ParserAction.h"

// define classical 'lists'
class ParserItem;
typedef QLinkedList<ParserItem *> ParserItemList;
typedef QMap<const char *, ParserItem *> ParserItemMap;

class ParserItem
{
public:
	typedef enum { TypeToken,		// item is a token (m_value_number is valid)
						TypeGreedy,		// item is a greedy token - read until this token (m_value_number is valid)
						TypeJump,		// item used to jump (m_jump_table is valid)
						TypePoint,		// item is an arrival point for Branch items
						TypeBegin,		// item is start of a sub routine
						TypeEnd,			// item is end of a sub routine
						TypeSub,		// item is a call to sub routine (m_value_string is valid)
						TypeCallback,	// item is a callback function
					 } ParserItemType_t;

	ParserItem(const char * rule, ParserItemType_t type);
	~ParserItem(void);

	// a unique identifier
	int Id(void) const throw (ParserException);

	// the next item (or nullptr if last item)
	ParserItem * Next(void) const throw (ParserException);

	// Update some value in the structure
	void Finalize(int id, ParserItem * next) throw (ParserException);

	// rule name where comes from the item
	inline const char * Rule(void) const { return m_rule; }

	// to handle type of item
	inline ParserItemType_t Type(void) const { return m_type; }
	const char * TypeStr(void) const;

	inline bool isToken(void) const { return Type() == TypeToken; }
	inline bool isGreedy(void) const { return Type() == TypeGreedy; }
	inline bool isJump(void) const { return Type() == TypeJump; }
	inline bool isPoint(void) const { return Type() == TypePoint; }
	inline bool isBegin(void) const { return Type() == TypeBegin; }
	inline bool isEnd(void) const { return Type() == TypeEnd; }
	inline bool isSub(void) const { return Type() == TypeSub; }
	inline bool isCallback(void) const { return Type() == TypeCallback; }

	// to handle token
	void Token(int token) throw (ParserException);
	int Token(void) const throw (ParserException);

	// to handle subroutine name
	void Sub(const char * sub) throw (ParserException);
	const char * Sub(void) const throw (ParserException);

	// to handle jump table
	void Jump(ParserItem * jump) throw (ParserException);
	const ParserItemList & Jump(void) const throw (ParserException);

	// to handle callback exec feature
	void Callback(ParserAction * callback, int Order);
	inline ParserAction * Callback(int & order) const { order = m_callback_order; return m_callback_fct; }

	void Dump(void) const;

private:
	ParserItemType_t m_type;
	const char * m_rule;
	int m_id;
	ParserItem * m_next;

	int m_value_number;
	const char * m_value_string;

	// jump table
	ParserItemList m_jump_table;

	// exec callback members
	ParserAction * m_callback_fct;
	int m_callback_order;

};

#endif // PARSERITEM_H
