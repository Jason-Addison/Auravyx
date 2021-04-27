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

	void registerThread(const std::thread::id& id, const std::string& name);

	std::string getThreadName(const std::thread::id& id);

private:

	static ThreadManager* threadManager;
	static std::map<std::thread::id, std::string> threads;

};

