#ifndef MIBCHECKERTHREAD_H
#define MIBCHECKERTHREAD_H

#include <QThread>
#include "MibCheck.h"

class MibCheckerThread : public QThread
{
	 Q_OBJECT

public:
	MibCheckerThread(void);
	~MibCheckerThread(void);

	bool Start(MibCheck::OverwriteAlgorithm Algo, const QStringList & MibList);

signals:
	void threadStarted(int Max);
	void threadRunning(int Pos, const QString Mib, const QString Error);
	void threadFinished(void);

protected:
	void run(void);

private:
	QStringList m_list_mib;
	MibCheck::OverwriteAlgorithm m_algo;

};

#endif // MIBCHECKERTHREAD_H
