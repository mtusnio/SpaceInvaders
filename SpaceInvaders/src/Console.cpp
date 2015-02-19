#include "Console.h"

#include <iostream>

void Console::Log(const std::string & str, Severity sev)
{
	if(sev == Severity::ERROR)
	{
		std::cout << "!!ERROR!!" << std::endl;
	}
	else if(sev == Severity::WARNING)
	{
		std::cout << "Warning: ";
	}


	std::cout << str << std::endl;
}

void Console::LogDebug(const std::string & str)
{
#ifdef _DEBUG
	Log(str);
#endif
}