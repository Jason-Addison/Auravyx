#pragma once
#include <thread>
#include <map>
#include <string>
class ThreadManager
{
public:

	ThreadManager();
	ThreadManager(ThreadManager* tm);

	static ThreadManager* getThreadManager();

	void registerThread(std::thread::id id, std::string name);

	std::string getThreadName(std::thread::id id);

private:

	static ThreadManager* threadManager;
	static std::map<std::thread::id, std::string> threads;

};

