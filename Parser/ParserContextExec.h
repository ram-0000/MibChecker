#ifndef PARSERCONTEXTEXEC_H
#define PARSERCONTEXTEXEC_H

#include "ParserItem.h"
#include "ParserValue.h"
#include <QVector>

class ParserContextExec
{
public:
	ParserContextExec(void);
	~ParserContextExec(void);

	void Reset(void);
	bool isSet(void) const;

	inline size_t CallStackSize(void) const {  return m_callstack.size(); }
	void CallStackPush(ParserItem * item);
	void CallStackPop(void);
	QString CallStackStr(void) const;

	ParserItem * PcGet(void) const;
	void PcSet(ParserItem * item);
	void PcNext(void);

	inline ParserItemList & getCallbackDelayed(void) { return m_callback_delayed; }
	inline QVector<ParserValue> & getValueDelayed(void) { return m_value_delayed; }

private:
	QVector<ParserValue> m_value_delayed;
	ParserItemList m_callback_delayed;
	ParserItemList m_callstack;
	ParserItem * m_pc;

};

#endif // PARSERCONTEXTEXEC_H
