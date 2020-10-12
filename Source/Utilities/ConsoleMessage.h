#pragma once
#include <string>
class ConsoleMessage
{
public:
	ConsoleMessage();
	~ConsoleMessage();

	enum Type
	{
		MESSAGE,
		WARNING_MESSAGE,
		ERROR_MESSAGE,
		CRITICAL_ERROR_MESSAGE
	};

	std::string text;
	std::string thread;
	std::string location;
	std::string time;

	char type = Type::MESSAGE;
	int id;

	int hours = 0;
	int minutes = 0;
	int seconds = 0;

};

