#include "Engine\OutputConsole.h"
#include <thread>
#include <string>
#include "Graphics/GFX.h"
#include "Utilities/Resource.h"
#include <atomic>
#include "Library/SOIL/SOIL.h"
#include "GUI/GUI.h"
#include "GUI/GUIContainer.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>
#include "Logger/Log.h"
#include "Engine/ThreadManager.h"
#include "Engine/GLManager.h"
std::chrono::duration<long, std::milli> timestep = std::chrono::milliseconds(30);
OutputConsole* OutputConsole::console;

OutputConsole::OutputConsole()
{
}

double FPS = 60;
int messageCount[5];
OutputConsole::OutputConsole(OutputConsole* c)
{
	console = c;
}

OutputConsole::~OutputConsole()
{
	for (int i = 0; i < 5; i++)
	{
		messageCount[i] = 0;
	}
}
GLFWwindow* outputWindow;
std::atomic_bool OutputConsole::ready = false;

double OutputConsole::scroll = 0;

bool OutputConsole::resized = true;


void OutputConsole::displayResizeCallback(GLFWwindow* _window, int _width, int _height)
{
	resized = true;
}

void OutputConsole::errorCallback(int error, const char* description)
{

}
void OutputConsole::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	scroll += yoffset * 200;
	if (scroll > 0)
	{
		scroll = 0;
	}
	FPS = 60;
}
void OutputConsole::fboCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
	console->width = width;
	console->height = height;
	console->render();
}
bool windowEntered = false;
void OutputConsole::mouseEnterCallback(GLFWwindow*, int enter)
{
	windowEntered = enter;
}
void OutputConsole::setupWindow()
{
	if (!glfwInit())
	{
		Log::criticalError("[GLFW] glfwInit() failed in output console.");
	}

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	height = mode->height * 0.7;
	width = height * 1.4;

	outputWindow = glfwCreateWindow(width, height, std::string("Auravyx Console").c_str(), NULL, NULL);

	glfwSetWindowPos(outputWindow, 10, mode->height / 2 - height / 2);

	GLFWimage icons[1];
	std::string str = Resource::getInstance().DIR + "\\Assets\\Textures\\iconf.png";
	icons[0].pixels = SOIL_load_image(str.c_str(), &icons[0].width, &icons[0].height, 0, SOIL_LOAD_RGBA);
	glfwSetWindowIcon(outputWindow, 1, icons);
	SOIL_free_image_data(icons[0].pixels);

	glfwSetWindowSizeCallback(outputWindow, &displayResizeCallback);
	glfwSetFramebufferSizeCallback(outputWindow, &fboCallback);
	glfwSetErrorCallback(&errorCallback);
	glfwSetScrollCallback(outputWindow, &scrollCallback);
	glfwSetCursorEnterCallback(outputWindow, &mouseEnterCallback);

	glfwSetWindowSizeLimits(outputWindow, 500, 500, GLFW_DONT_CARE, GLFW_DONT_CARE);
	glfwMakeContextCurrent(outputWindow);
	glfwSwapInterval(0);
	glfwShowWindow(outputWindow);
	glfwSwapBuffers(outputWindow);
	glfwPollEvents();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glewExperimental = GL_TRUE;

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		printf("[Mod] GLEW Error : %s\n", glewGetErrorString(error));
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(GLManager::glCallback, 0);
	glViewport(0, 0, width, height);

	GFX::getOverlay()->init();

	Resource::getInstance().loadBootAssets();

	glfwMakeContextCurrent(outputWindow);
	OutputConsole::ready = true;
}

void OutputConsole::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.15, 0.15, 0.15, 1);
	glViewport(0, 0, width, height);
	if (true)
	{
		//scroll = -10000000;
	}
	if (-((int)scroll - height + 125) > (long int)messages.size() * 25)
	{
		scroll = (((long int)messages.size() * 25) - height + 125);
		scroll = -scroll;
		if (messages.size() < (height - 125) / 25)
		{
			scroll = 0;
		}
		if (scroll > 0)
		{
			scroll = 0;
		}
	}

	int start = -scroll / 25;
	int end = start + (height - 100 + 25) / 25;
	if (end > messages.size())
	{
		end = messages.size();
	}
	int e = end - start;
	int offset = 155;

	if (width > 1000)
	{
		offset = 455;
	}
	for (int i = start; i < end; i++)
	{
		if (messages.at(i).id % 2 == 1)
		{
			if (messages.at(i).type == ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE)
			{
				GFX::getOverlay()->fillRect(0, height - i * 25 - 100 - scroll, width, 25, width, height, 0.75, 0, 0, 1);
			}
			else
			{
				GFX::getOverlay()->fillRect(0, height - i * 25 - 100 - scroll, width, 25, width, height, 0.12, 0.12, 0.12, 1);
			}
		}
		else
		{
			if (messages.at(i).type == ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE)
			{
				GFX::getOverlay()->fillRect(0, height - i * 25 - 100 - scroll, width, 25, width, height, 0.7, 0, 0, 1);
			}
			else
			{
				GFX::getOverlay()->fillRect(0, height - i * 25 - 100 - scroll, width, 25, width, height, 0.14, 0.14, 0.14, 1);
			}
		}
		GFX::getOverlay()->drawString(messages.at(i).time, 5, (i * 25) + 80 + scroll, 21, 1, 1, 1, 1, width, height);


		if (width > 1000)
		{
			GFX::getOverlay()->drawString(messages.at(i).location, 155, (i * 25) + 80 + scroll, 21, 1, 1, 1, 1, width, height);

			if (messages.at(i).thread.size() > 0)
			{
				GFX::getOverlay()->drawString(messages.at(i).thread, 305, (i * 25) + 80 + scroll, 21, 1, 1, 1, 1, width, height);
			}
			else
			{
				GFX::getOverlay()->drawString("Unknown", 305, (i * 25) + 80 + scroll, 21, 0.7, 0.7, 0.7, 1, width, height);
			}
		}


		switch (messages.at(i).type)
		{
		case(ConsoleMessage::Type::MESSAGE):
		{
			GFX::getOverlay()->drawString(messages.at(i).text, offset, (i * 25) + 80 + scroll, 21, 1, 1, 1, 1, width, height);
			break;
		}
		case(ConsoleMessage::Type::WARNING_MESSAGE):
		{
			GFX::getOverlay()->drawString(messages.at(i).text, offset, (i * 25) + 80 + scroll, 21, 1, 0.6, 0, 1, width, height);
			break;
		}
		case(ConsoleMessage::Type::ERROR_MESSAGE):
		{
			GFX::getOverlay()->drawString(messages.at(i).text, offset, (i * 25) + 80 + scroll, 21, 1, 0.2, 0.2, 1, width, height);
			break;
		}
		case(ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE):
		{
			GFX::getOverlay()->drawString(messages.at(i).text, offset, (i * 25) + 80 + scroll, 21, 1, 1, 1, 1, width, height);
			break;
		}
		case(ConsoleMessage::Type::DEBUG_MESSAGE):
		{
			GFX::getOverlay()->drawString(messages.at(i).text, offset, (i * 25) + 80 + scroll, 21, 0.5, 0.5, 0.5, 1, width, height);
			break;
		}
		}
	}

	GFX::getOverlay()->fillRect(width - 15, 50, 15, height - 100, width, height, 0.1, 0.1, 0.1, 1);

	if (messages.size() > 0)
	{
		double barSize = (double)((height - 125) / 25) / (double)messages.size();
		if (barSize > 1)
		{
			barSize = 1;
		}
		double sb = (height - 125 - 4) * (barSize);
		if (sb < 5)
		{
			sb = 5;
		}
		GFX::getOverlay()->fillRect(width - 12, height - 75 - (sb)-(-scroll * barSize) + -2, 10, sb, width, height, 1, 1, 1, 1);
	}

	GFX::getOverlay()->fillRect(0, 0, width, 50, width, height, 0.10, 0.10, 0.10, 1);
	GFX::getOverlay()->fillRect(0, height - 75, width, 75, width, height, 0.1, 0.1, 0.1, 1);

	GFX::getOverlay()->drawString("Time", 5, 55, 21, 1, 1, 1, 1, width, height);

	if (width > 1000)
	{
		GFX::getOverlay()->drawString("Location", 155, 55, 21, 1, 1, 1, 1, width, height);

		GFX::getOverlay()->drawString("Thread", 305, 55, 21, 1, 1, 1, 1, width, height);
	}

	GFX::getOverlay()->drawString("Message", offset, 55, 21, 1, 1, 1, 1, width, height);

	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	auto timer = std::chrono::system_clock::to_time_t(now);
	std::tm bt = *std::localtime(&timer);
	std::ostringstream oss;
	oss << std::put_time(&bt, "%H:%M:%S");
	oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

	GFX::getOverlay()->drawString("Time : " + oss.str(), 10, 10, 21, 1, 1, 1, 1, width, height);
	GFX::getOverlay()->drawString("Runtime : " + Util::removeDecimal(glfwGetTime(), 1) + "s", 10, 30, 21, 1, 1, 1, 1, width, height);

	GFX::getOverlay()->drawString("Total Messages : " + std::to_string(messageCount[ConsoleMessage::Type::MESSAGE] + messageCount[ConsoleMessage::Type::WARNING_MESSAGE]
		+ messageCount[ConsoleMessage::Type::ERROR_MESSAGE] + messageCount[ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE] + messageCount[ConsoleMessage::Type::DEBUG_MESSAGE]
	), 305, 10, 21, 1, 1, 1, 1, width, height);
	GFX::getOverlay()->drawString("Regular : " + std::to_string(messageCount[ConsoleMessage::Type::MESSAGE]), 505, 10, 21, 1, 1, 1, 1, width, height);
	GFX::getOverlay()->drawString("Warnings : " + std::to_string(messageCount[ConsoleMessage::Type::WARNING_MESSAGE]), 635, 10, 21, 1, 1, 1, 1, width, height);
	GFX::getOverlay()->drawString("Errors : " + std::to_string(messageCount[ConsoleMessage::Type::ERROR_MESSAGE]), 765, 10, 21, 1, 1, 1, 1, width, height);
	GFX::getOverlay()->drawString("Critical Errors : " + std::to_string(messageCount[ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE]), 895, 10, 21, 1, 1, 1, 1, width, height);
	GFX::getOverlay()->drawString("Debug : " + std::to_string(messageCount[ConsoleMessage::Type::DEBUG_MESSAGE]), 1075, 10, 21, 1, 1, 1, 1, width, height);

	glfwSwapBuffers(outputWindow);
	glfwPollEvents();

	if (resized)
	{
		glfwGetWindowSize(outputWindow, &width, &height);
		resized = false;
	}
	int state = glfwGetKey(outputWindow, GLFW_KEY_E);
	if (state == GLFW_PRESS)
	{
		message(std::to_string(glfwGetTime()), ConsoleMessage::Type::CRITICAL_ERROR_MESSAGE);
	}
	state = glfwGetKey(outputWindow, GLFW_KEY_T);
	if (state == GLFW_PRESS)
	{
		message(std::to_string(glfwGetTime()), ConsoleMessage::Type::ERROR_MESSAGE);
	}
	state = glfwGetKey(outputWindow, GLFW_KEY_Y);
	if (state == GLFW_PRESS)
	{
		message(std::to_string(glfwGetTime()), ConsoleMessage::Type::WARNING_MESSAGE);
	}
	std::this_thread::sleep_for(timestep);
}

void OutputConsole::consoleFunction()
{
	ThreadManager::getThreadManager()->registerThread(std::this_thread::get_id(), "Console");
	setupWindow();
	double last = 0;
	double now = 0;

	while (true)
	{
		now = glfwGetTime();
		if (now - last > 1.0 / FPS)
		{
			last = now;
			if (!addingMessage)
			{
				if (windowEntered)
				{
					FPS = 60;
				}
				else if (messagesToLoad.size() > 0)
				{
					FPS = 30;
				}
				else
				{
					FPS = 15;
				}
				messages.insert(messages.end(), messagesToLoad.begin(), messagesToLoad.end());
				messagesToLoad.clear();
			}

			render();
			if (glfwWindowShouldClose(outputWindow))
			{
				OutputConsole::getConsole()->shutdown = true;
				if (cleanupFinished)
				{
					break;
				}
			}
		}
		else
		{
			std::this_thread::sleep_for(timestep);
		}
	}
	cleanupFinished = false;
}
std::thread consoleThread;
void OutputConsole::start()
{
	consoleThread = std::thread(&OutputConsole::consoleFunction, getConsole());
}

void OutputConsole::stop()
{
}

bool OutputConsole::isReady()
{
	bool r = OutputConsole::ready;
	return r;
}

void OutputConsole::message(const std::string& msg, const int level)
{
	ConsoleMessage m;
	auto now = std::chrono::system_clock::now();
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
	auto timer = std::chrono::system_clock::to_time_t(now);
	std::tm bt = *std::localtime(&timer);
	std::ostringstream oss;
	oss << std::put_time(&bt, "%H:%M:%S");
	oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

	m.time = oss.str();

	static int i = 0;
	m.text = msg;
	m.id = i++;
	m.type = level;
	messageCount[level]++;
	m.location = "Auravyx";
	m.thread = ThreadManager::getThreadManager()->getThreadName(std::this_thread::get_id());

	OutputConsole::getConsole()->addingMessage = true;
	OutputConsole::getConsole()->messagesToLoad.emplace_back(m);
	OutputConsole::getConsole()->addingMessage = false;
}

OutputConsole* OutputConsole::getConsole()
{
	return console;
}

void OutputConsole::cleanup()
{
	cleanupFinished = true;
	while (cleanupFinished)
	{
		std::this_thread::sleep_for(timestep);
	}
	glfwPollEvents();
	glfwDestroyWindow(outputWindow);
	consoleThread.join();
}