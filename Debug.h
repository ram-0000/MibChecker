#ifndef DEBUG_H
#define DEBUG_H

#include <QString>
#include "Windows.h"

#define DEBUG(...) Debug::Log(__FILE__, __LINE__, Debug::LogDebug, __VA_ARGS__)
#define INFO(...) Debug::Log(__FILE__, __LINE__, Debug::LogInfo, __VA_ARGS__)
#define USER(...) Debug::Log(__FILE__, __LINE__, Debug::LogUser, __VA_ARGS__)
#define WARNING(...) Debug::Log(__FILE__, __LINE__, Debug::LogWarning, __VA_ARGS__)
#define ERREUR(...) Debug::Log(__FILE__, __LINE__, Debug::LogError, __VA_ARGS__)
#define CRITICAL(...) Debug::Log(__FILE__, __LINE__, Debug::LogCritical, __VA_ARGS__)
#define FATAL(...) Debug::Log(__FILE__, __LINE__, Debug::LogFatal, __VA_ARGS__)

#define WINDOWS(Fct,Code) Debug::WindowsError(__FILE__, __LINE__, Fct, Code)

class Debug
{
public:
	typedef enum
	{	LogDebug,
		LogInfo,
		LogUser,
		LogWarning,
		LogError,
		LogCritical,
		LogFatal
	} LogLevel_t;

	static void WindowsError(const char * File, int Line, const char * FunctionName, const LONG ErrorCode);

	static void Log(const char * File, int Line, LogLevel_t Level, const char * Format, ...);

protected:

private:
	Debug(void);
	static const char * StrLevel(LogLevel_t Level);
	static const char * ShortFile(const char * File);
};

#endif // DEBUG_H
