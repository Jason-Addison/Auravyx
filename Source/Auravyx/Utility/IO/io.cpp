#include "io.h"
#include "Logger/OutputConsole.h"
io::io()
{
}

io& io::operator<<(const std::string& msg)
{
	///OutputConsole::getConsole()->message(msg, 0);
	return *this;
}

io io::out;
