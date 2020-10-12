#include "ThreadManager.h"

std::map<std::thread::id, std::string> ThreadManager::threads;
ThreadManager* ThreadManager::threadManager;
ThreadManager::ThreadManager()
{
}

ThreadManager::ThreadManager(ThreadManager* tm)
{
	threadManager = tm;
}

ThreadManager* ThreadManager::getThreadManager()
{
	return threadManager;
}

void ThreadManager::registerThread(std::thread::id id, std::string name)
{
	threads.emplace(id, name);
}

std::string ThreadManager::getThreadName(std::thread::id id)
{
	std::string s = "";
	std::map<std::thread::id, std::string>::const_iterator iter = threads.find(id);

	if (iter != threads.end())
	{
		s = iter->second;
	}
	return s;
}
