#include "Debug.h"

#include "windows.h"

// because MAKELANGID in Winnt.h generate warning regarding old style casting
#define MY_MAKELANGID(p,s) (((static_cast<WORD>(s)) << 10) | static_cast<WORD>(p))

void Debug::WindowsError(const char * File, int Line, const char * FunctionName, const LONG ErrorCode)
{
	// get error message
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
					  nullptr,
					  static_cast<DWORD>(ErrorCode),
					  MY_MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
					  reinterpret_cast<LPTSTR>(&lpMsgBuf),
					  0,
					  nullptr );
	QString str(reinterpret_cast<QChar *>(lpMsgBuf));
	LocalFree(lpMsgBuf);

	Log(File, Line, LogError, "%s failed with error %d: %s", FunctionName, ErrorCode, str.toUtf8().constData());
}

void Debug::Log(const char * File, int Line, LogLevel_t Level, const char * Format, ...)
{
	// prepare the message itself
	va_list  pa;
	va_start(pa, Format);
	char msg[1024];
	vsnprintf(msg, sizeof(msg), Format, pa);
	va_end(pa);

	// prepare the header of the message
	char message[1024];
	snprintf(message, sizeof(message), "%s ==> [%s:%d] %s",
				StrLevel(Level),
				ShortFile(File),
				Line,
				msg);

	// output the message
	switch(Level)
	{
		case LogDebug: qDebug(message); return;
		case LogInfo: qInfo(message); return;
		case LogUser: qInfo(message); return;
		case LogWarning: qWarning(message); return;
		case LogError: qWarning(message); return;
		case LogCritical: qCritical(message); return;
		case LogFatal: qFatal(message); return;
	}
}

const char * Debug::StrLevel(LogLevel_t Level)
{
	switch(Level)
	{
		case LogDebug: return "Debug";
		case LogInfo: return "Info";
		case LogUser: return "User";
		case LogWarning: return "Warning";
		case LogError: return "Error";
		case LogCritical: return "Critical";
		case LogFatal: return "Fatal";
	}
	return "Unknown";
}

const char * Debug::ShortFile(const char * File)
{
	// try to find last
	const char *p = strrchr(File, '\\');
	if(p != nullptr)
		return p + 1;

	// try to find last
	p = strrchr(File, '/');
	if(p != nullptr)
		return p + 1;

	// nothing found, return entry
	return File;
}
