#ifndef H_CONSOLE
#define H_CONSOLE

#include <string>

class Console
{
public:
	enum Severity
	{
		LOG,
		WARNING,
		ERROR
	};

	static void Log(const std::string & str, Severity sev = LOG);
	static void LogDebug(const std::string & str);
};


#endif