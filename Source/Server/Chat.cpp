#include "Chat.h"
#include "Controller.h"
#include "WindowManager.h"
#include <Log.h>
#include "GFX.h"
#include "Clipboard.hpp"
#include <ChunkIO.h>
#include "SOIL/SOIL.h"
#include <Resource.h>
#include <iomanip>
#include <iostream>
Chat::Chat()
{
}


Chat::~Chat()
{
}

bool Chat::isChatting = false;

std::string currentMessage;
std::vector<std::string> chatLog;
std::vector<float> chatLogTimings;

bool backspacePriority = false;

int selectionStart = 0;
int selectionEnd = 0;

void noTextCallback(GLFWwindow* window, unsigned int codepoint)
{

}

void textCallback(GLFWwindow* window, unsigned int codepoint)
{
	backspacePriority = false;
	if (!backspacePriority)
	{
		char16_t c = codepoint;
		currentMessage += (char)c;
	}

}

void noKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}


void chatKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (Chat::isChatting)
	{
		if (action)
		{
			if (key == GLFW_KEY_BACKSPACE)
			{
				backspacePriority = true;
				if (backspacePriority && currentMessage.length() > 0)
				{
					//std::string bmsg = currentMessage.str();
					//bmsg = bmsg.substr(0, bmsg.length() - 1);
					//currentMessage.str(bmsg);
					currentMessage = currentMessage.substr(0, currentMessage.length() - 1);
				}
			}
			if (key == 65 && mods == 2)
			{
				selectionStart = 0;
				selectionEnd = currentMessage.length();
			}
			if (key == 67 && mods == 2)
			{
				try 
				{
					if (selectionEnd > selectionStart)
					{
						Clipboard clipboard;

						clipboard << currentMessage.substr(selectionStart, selectionEnd).c_str();
					}
				}
				catch (CExceptionXX& e) 
				{
					std::cout << e.what() << std::endl;
				}
			}
			if (key == 86 && mods == 2)
			{
				try
				{
					Clipboard clipboard;
					std::string pasted = "";
					//clipboard << currentMessage.substr(selectionStart, selectionEnd).c_str();
					clipboard >> pasted;
					if (pasted.length() > 0)
					{
						currentMessage += pasted;
					}
				}
				catch (CExceptionXX & e)
				{
					std::cout << e.what() << std::endl;
				}
			}
		}
			// paste
			//std::string paste_text;
			//clipboard >> paste_text;
	}
}



void Chat::update()
{
	
}

double lastTime = 0;
double thisTime = 0;

double textFlashSpeed = 0.3;

bool textFlashOn = false;

bool lastMode = false;

bool f1Lock = false;

void Chat::render()
{
	lastMode = isChatting;
	if (!isChatting && Controller::isKeyDown(GLFW_KEY_T))
	{
		isChatting = true;
		WindowManager::setTextCallback(textCallback);
		WindowManager::setKeyCallback(chatKeyCallback);
	}
	if (isChatting && Controller::isKeyDown(GLFW_KEY_ESCAPE))
	{
		WindowManager::setTextCallback(noTextCallback);
		WindowManager::setKeyCallback(noKeyCallback);
		isChatting = false;
		currentMessage = std::string();
	}
	if (isChatting && Controller::isKeyDown(GLFW_KEY_ENTER))
	{
		WindowManager::setTextCallback(noTextCallback);
		WindowManager::setKeyCallback(noKeyCallback);
		if (currentMessage.length() > 0)
		{
			std::string outputMessage = currentMessage;
			//outputMessage = outputMessage.substr(0, outputMessage.length() - 1);
			if (outputMessage.at(0) == '/')
			{
				command(outputMessage.substr(1, outputMessage.length()));
			}
			else
			{
				message(outputMessage);
			}
			currentMessage = std::string();
		}
		isChatting = false;
	}
	if (isChatting != lastMode)
	{
		if (isChatting)
		{
			WindowManager::centerCursor();
			WindowManager::centerCursor();
			WindowManager::showMouse();
			WindowManager::centerCursor();
		}
		if (!isChatting)
		{
			WindowManager::hideMouse();
		}
	}
	thisTime = glfwGetTime();
	if (thisTime > lastTime + textFlashSpeed)
	{
		lastTime = thisTime;
		textFlashOn = !textFlashOn;
	}
	if (isChatting)
	{
		GFX::fillRect(5, WindowManager::height - 35, WindowManager::width - 10, 30, 0, 0, 0, 0.5);
		std::string outmsg = "";
		outmsg += currentMessage;
		if (textFlashOn)
		{
			outmsg += "_";
		}
		GFX::drawString(outmsg, 7, WindowManager::height - 32.5, 30, 1, 1, 1, 1);
		
	}
	if (!f1Lock && Controller::isKeyDown(GLFW_KEY_F1))
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		f1Lock = true;
		std::string outFile = Resource::DIR + "//Screenshots//";
		std::stringstream ss;
		ss << Resource::DIR + "//Screenshots//";
		ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		ss << ".png";
		int save_result = SOIL_save_screenshot
		(
			ss.str().c_str(),
			SOIL_SAVE_TYPE_BMP,
			0, 0, WindowManager::getWidth(), WindowManager::getHeight()
		);
		message(ss.str());
	}
	else if (!Controller::isKeyDown(GLFW_KEY_F1))
	{
		f1Lock = false;
	}
	
	int toRemove = 0;
	float thisTime = glfwGetTime();
	for (int i = (int) chatLog.size() - 1; i >= 0; i--)
	{
		GFX::drawStringBG(chatLog.at(i), 5, WindowManager::height - 65 - (chatLog.size() - i) * 30, 30, 1, 1, 1, 1, 0, -2, 0, 0, 0, 0, 0, 0.3);
	}
	for (int i = 0; i < chatLogTimings.size(); i++)
	{
		if (chatLogTimings.at(i) + 10 < thisTime)
		{
			toRemove++;
		}
	}
	for (int i = 0; i < toRemove; i++)
	{
		chatLog.erase(chatLog.begin());
		chatLogTimings.erase(chatLogTimings.begin());
	}
}

void Chat::message(std::string msg)
{
	chatLog.emplace_back(msg);
	chatLogTimings.emplace_back(glfwGetTime());
	Log::out("Chat", msg, YELLOW);
}

void Chat::command(std::string cmd)
{
	std::vector<std::string> parse = Util::splitString(cmd, " ");
	if (parse.size() > 1 && parse.at(0).compare("speed") == 0)
	{
		float speed = std::stof(parse.at(1));
		Log::out("Command", "Speed set to " + std::to_string(speed), RED);
		GFX::CAM.setSpeedMultiplier(speed);
	}
	if (parse.size() > 1 && parse.at(0).compare("fov") == 0)
	{
		float fov = std::stof(parse.at(1));
		Log::out("Command", "FOV set to " + std::to_string(fov), RED);
		GFX::CAM.setFOV(fov);
	}
	if (parse.size() > 1 && parse.at(0).compare("fps") == 0)
	{
		float fps = std::stof(parse.at(1));
		Log::out("Command", "FPS set to " + std::to_string(fps), RED);
		GFX::FPS = fps;
	}
	if (parse.size() > 0 && parse.at(0).compare("save") == 0)
	{
		Log::out("Command", "Saving...", RED);
		ChunkIO io;
		for (auto c : w->overworld)
		{
			io.saveChunk(c);
		}
		Log::out("Command", "Save complete.", RED);
	}
}
