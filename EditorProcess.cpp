#include "EditorProcess.h"
#include "Conf.h"
#include <QProcess>

EditorProcess::EditorProcess(const QString & Folder, const QString & File)
{
	QString program;
	program += "\"";
	program += Conf::EditorPath();
	program += "\" \"";
	program += Folder;
	program += "\\";
	program += File;
	program += "\"";

	QProcess * proc = new QProcess();
	proc->start(program);
}
