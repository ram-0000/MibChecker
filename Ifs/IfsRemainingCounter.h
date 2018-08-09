#ifndef IFS_REMAININGCOUNTER_H
#define IFS_REMAININGCOUNTER_H

#include <QtGlobal>
#include <QVector>

class IfsRemainingCounter
{
public:
	IfsRemainingCounter(void);
	~IfsRemainingCounter(void);
	void Reset(void);

	void Push(quint32 InitialValue);
	bool Pop(void);
	bool Decremente(quint32 value);
	bool Decremente(void);
	quint32 Last(void) const;
	inline bool IsEmpty(void) const { return m_table.size() == 0; }

private:
	QVector<quint32> m_table;
};

#endif // IFS_REMAININGCOUNTER_H
