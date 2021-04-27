#pragma once
#include "Library/GL/glew.h"
#include "Library/GLFW/glfw3.h"
#include <atomic>
#include <string>
#include <vector>
#include "Utilities/ConsoleMessage.h"
class OutputConsole
{
public:

	OutputConsole();
	OutputConsole(OutputConsole* c);
	~OutputConsole();

	void start();

	void stop();

	bool isReady();
	static std::atomic_bool ready;

	void message(const std::string& msg, const int level);

	static OutputConsole* getConsole();

	std::atomic_bool shutdown = false;

	void cleanup();

	std::atomic_bool addingMessage = false;

	std::vector<ConsoleMessage> messagesToLoad;

	std::vector<ConsoleMessage> messages;

private:

	std::atomic_bool cleanupFinished = false;

	int width = 1000;
	int height = 800;

	static double scroll;

	static bool resized;
	static void displayResizeCallback(GLFWwindow* _window, int _width, int _height);
	static void errorCallback(int error, const char* description);
	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouseEnterCallback(GLFWwindow*, int enter);
	void setupWindow();
	void consoleFunction();

	static OutputConsole* console;

	static void fboCallback(GLFWwindow* window, int width, int height);

	void render();
};