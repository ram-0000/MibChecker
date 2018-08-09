#include "MibCheckerThread.h"
#include "Debug.h"

MibCheckerThread::MibCheckerThread(void)
{
	m_list_mib.clear();
}

MibCheckerThread::~MibCheckerThread(void)
{
	m_list_mib.clear();
}

bool MibCheckerThread::Start(MibCheck::OverwriteAlgorithm Algo,
									  const QStringList & MibList)
{
	if(isRunning() == true)
	{
		// already started
		return false;
	}

	if(MibList.size() == 0)
		return false;

	m_list_mib.clear();
	m_list_mib = MibList;
	m_algo = Algo;

	// start the thread
	start();
	return true;
}

void MibCheckerThread::run(void)
{
	DEBUG("Thread is started");
	emit threadStarted(m_list_mib.size());

	DEBUG("Thread is running");
	MibCheck mib;
	QString error;
	int pos = 0;
	for(const auto & item : m_list_mib)
	{
		error = "";
		if(mib.Check(m_algo, item) == false)
			error = mib.Error();

		pos++;
		emit threadRunning(pos, item, error);
	}

	DEBUG("Thread is finished");
	m_list_mib.clear();
	emit threadFinished();
}
