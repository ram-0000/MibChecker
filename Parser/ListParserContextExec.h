#ifndef LISTPARSERCONTEXTEXEC_H
#define LISTPARSERCONTEXTEXEC_H

#include <QVector>
#include "ParserItem.h"
#include "ParserContextExec.h"

class ListParserContextExec
{
public:
	ListParserContextExec(ParserItem * entry);
	~ListParserContextExec(void);

	inline int size(void) const { return m_list.size(); }

	ParserContextExec & Current(void);
	void CurrentRemove(void);
	void CurrentDuplicate(ParserItem * item);

	void CurrentIntoFound(void);
	void FoundIntoCurrent(void);
	void FoundReset(void);
	bool IsFound(void) const;
	ParserContextExec & Found(void);

private:
	QVector<ParserContextExec> m_list;

	ParserContextExec m_found;
};

#endif // LISTPARSERCONTEXTEXEC_H
