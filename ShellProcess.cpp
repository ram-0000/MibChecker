#include "ShellProcess.h"
#include "Debug.h"
#include "Conf.h"

ShellProcess::ShellProcess(const QString & path)
{
	STARTUPINFOA si;		// not used but mandatory
	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;		// not used but mandatory
	memset(&pi, 0, sizeof(pi));

	CreateProcessA(nullptr,
						Conf::CmdPath().toLatin1().data(),
						nullptr,
						nullptr,
						TRUE,
						0,
						nullptr,
						path.toLatin1().data(),
						&si,
						&pi);
}
