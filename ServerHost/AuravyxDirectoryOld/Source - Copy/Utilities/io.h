#pragma once
#include <string>
class io
{
public:

	io();

	io& operator <<(const std::string& msg);
	//static io& out(int n);

	static io out;
};

