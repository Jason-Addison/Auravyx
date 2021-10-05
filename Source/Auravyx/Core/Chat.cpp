#include "Auravyx/Core/Chat.h"
#include "Auravyx/UI/Control/Controller.h"
#include "Auravyx/UI/Window.h"
#include <Logger/Log.h>
#include <Auravyx/Core/World/ChunkIO.h>
#include "SOIL/SOIL.h"
#include <Auravyx/Utility/IO/Resource.h>
#include <Auravyx/Graphics/GFX.h>
#include <Auravyx/Core/Command/CommandRegistry.h>
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
std::atomic_bool chatChanged = false;

int selectionStart = 0;
int selectionEnd = 0;

void noTextCallback(GLFWwindow* window, unsigned int codepoint)
{

}

struct CommandSuggestion
{
	std::string name;
	int index;
};

std::vector<CommandSuggestion> suggestionList;
int suggestionIndex = 0;
bool commandError = false;
int currentSuggestionIndex = 0;
int currentRealSuggestionIndex = 0;
std::vector<CommandSuggestion> autofillList;

void refreshSuggestions()
{
	currentSuggestionIndex = 1;
	commandError = false;
	suggestionList.clear();

	int j = currentMessage.find_last_of(' ');
	if (j <= 0)
	{
		j = 1;
	}
	else
	{
		j += 1;
	}
	std::string currentArgument = currentMessage.substr(j, currentMessage.length());

	int entries = 0;
	bool midArgument = (currentArgument.length() != 0);
	if (currentMessage.size() > 0 && currentMessage.at(0) == '/')
	{
		std::vector<std::string> parse = Util::splitString(currentMessage.substr(1, currentMessage.length()), " ");

		Command cmd;
		int i = 0;
		if (parse.size() <= 1 && currentMessage.at(currentMessage.length() - 1) != ' ')
		{
			std::map<std::string, Command>::iterator itr;
			for (itr = CommandRegistry::commands.begin(); itr != CommandRegistry::commands.end(); itr++)
			{
				CommandSuggestion cs;
				cs.name = itr->first;
				cs.index = itr->second.argumentType;
				suggestionList.emplace_back(cs);
			}
		}
		else
		{
			while (i < parse.size())
			{
				if (i <= 0)
				{
					if (CommandRegistry::commands.find(parse.at(i)) != CommandRegistry::commands.end())
					{
						cmd = CommandRegistry::commands.at(parse.at(i));
						currentSuggestionIndex += parse.at(i).size() + 1;
					}
					else
					{
						commandError = true;
					}
				}
				else
				{
					bool cmdFound = false;
					for (int j = 0; j < cmd.subCommands.size(); j++)
					{
						if (cmd.subCommands.at(j).validArgument(parse.at(i)))
						{
							bool suggestionFound = false;
							if (cmd.subCommands.at(j).suggestions)
							{
								for (int k = 0; k < cmd.subCommands.at(j).suggestions->size(); k++)
								{
									if (cmd.subCommands.at(j).suggestions->at(k).rfind(parse.at(i), 0) == 0 && currentMessage.at(currentMessage.length() - 1) != ' ')
									{
										CommandSuggestion cs;
										cs.name = cmd.subCommands.at(j).suggestions->at(k);
										cs.index = Command::Argument::SUGGESTION;
										suggestionList.emplace_back(cs);
										suggestionFound = true;
									}
								}
							}
							if (i < parse.size() - 1 || currentMessage.at(currentMessage.length() - 1) == ' ')
							{
								cmd = cmd.subCommands.at(j);
								cmdFound = true;
								if (!suggestionFound)
								{
									currentSuggestionIndex += parse.at(i).size() + 1;
								}
							}
							else
							{
								cmdFound = true;
							}
							break;
						}
						if (cmd.subCommands.at(j).argumentType == Command::Argument::LITERAL &&
							cmd.subCommands.at(j).argumentValue.rfind(parse.at(i), 0) == 0 &&
							cmd.subCommands.at(j).argumentValue.length() >= parse.at(i).length())
						{
							cmdFound = true;
							break;
						}
					}
					if (!cmdFound)
					{
						commandError = true;
						break;
					}
				}
				i++;
			}
			
			for (int i = 0; i < cmd.subCommands.size(); i++)
			{
				CommandSuggestion cs;
				cs.name = cmd.subCommands.at(i).argumentValue;
				cs.index = cmd.subCommands.at(i).argumentType;
				suggestionList.emplace_back(cs);
				if (currentMessage.at(currentMessage.length() - 1) == ' ')
				{
					if (cmd.subCommands.at(i).suggestions)
					{
						for (int j = 0; j < cmd.subCommands.at(i).suggestions->size(); j++)
						{
							CommandSuggestion css;
							css.name = cmd.subCommands.at(i).suggestions->at(j);
							css.index = Command::Argument::SUGGESTION;
							suggestionList.emplace_back(css);
						}
					}
				}
			}
		}
	}
	autofillList = std::vector<CommandSuggestion>();
	for (int i = suggestionList.size() - 1; i >= 0; i--)
	{
		if (suggestionList.at(i).name.rfind(currentArgument, 0) == 0 ||
			(midArgument &&
				Command::validArgument(currentArgument, suggestionList.at(i).index)))
		{
			if (!commandError)
			{
				autofillList.emplace_back(suggestionList.at(i));
			}
		}
	}
}

void textCallback(GLFWwindow* window, unsigned int codepoint)
{
	backspacePriority = false;
	if (!backspacePriority)
	{
		char16_t c = codepoint;
		currentMessage += (char)c;
	}
	chatChanged = true;
}

void noKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
}

int autofillIndex = -1;
std::string currentAutofill;
std::string currentSuggestionMsg;
void chatKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (Chat::isChatting)
	{
		if (action)
		{
			if (key == GLFW_KEY_TAB)
			{
				int newIndex = suggestionIndex;
				while (autofillList.at(newIndex).index != Command::Argument::LITERAL &&
					autofillList.at(newIndex).index != Command::Argument::SUGGESTION)
				{
					newIndex++;
					if (newIndex == autofillList.size())
					{
						newIndex = 0;
						break;
					}
				}
				suggestionIndex = newIndex;
				if (autofillIndex == suggestionIndex)
				{
					suggestionIndex++;
					if (suggestionIndex >= autofillList.size())
					{
						suggestionIndex = 0;
					}
					currentAutofill = autofillList.at(suggestionIndex).name;
				}
				currentAutofill = autofillList.at(suggestionIndex).name;
				autofillIndex = suggestionIndex;
				
			}
			else if (key == GLFW_KEY_UP)
			{
				suggestionIndex--;
				if (suggestionIndex < 0)
				{
					suggestionIndex = autofillList.size() - 1;
				}
				currentSuggestionMsg = autofillList.at(suggestionIndex).name;
			}
			else if (key == GLFW_KEY_DOWN)
			{
				suggestionIndex++;
				if (suggestionIndex >= autofillList.size())
				{
					suggestionIndex = 0;
				}
				currentSuggestionMsg = autofillList.at(suggestionIndex).name;
			}
			else
			{
				currentMessage += currentAutofill;
				currentAutofill = "";
				suggestionIndex = 0;
				autofillIndex = -1;
			}
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
				/*try
				{
					if (selectionEnd > selectionStart)
					{
						//Clipboard clipboard;

						//clipboard << currentMessage.substr(selectionStart, selectionEnd).c_str();
					}
				}
				catch (CExceptionXX& e) 
				{
					Log::out(e.what());
				}*/
			}
			if (key == 86 && mods == 2)
			{
				/*try
				{
					//Clipboard clipboard;
					std::string pasted = "";
					//clipboard << currentMessage.substr(selectionStart, selectionEnd).c_str();
					//clipboard >> pasted;
					//if (pasted.length() > 0)
					{
						currentMessage += pasted;
					}
				}
				catch (CExceptionXX & e)
				{
					Log::out(e.what());
				}*/
			}
			chatChanged = true;
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
	if (!isChatting && Window::getWindow()->getController()->isKeyDown(GLFW_KEY_T))
	{
		isChatting = true;
		Window::getWindow()->setTextCallback(textCallback);
		Window::getWindow()->setKeyCallback(chatKeyCallback);
	}
	if (isChatting && Window::getWindow()->getController()->isKeyDown(GLFW_KEY_ESCAPE))
	{
		Window::getWindow()->setTextCallback(noTextCallback);
		Window::getWindow()->setKeyCallback(noKeyCallback);
		isChatting = false;
		currentMessage = std::string();
	}
	if (isChatting && Window::getWindow()->getController()->isKeyDown(GLFW_KEY_ENTER))
	{
		Window::getWindow()->setTextCallback(noTextCallback);
		Window::getWindow()->setKeyCallback(noKeyCallback);
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
			Window::getWindow()->centerCursor();
			Window::getWindow()->centerCursor();
			Window::getWindow()->showMouse();
			Window::getWindow()->centerCursor();
		}
		if (!isChatting)
		{
			Window::getWindow()->hideMouse();
		}
	}
	thisTime = glfwGetTime();
	if (thisTime > lastTime + textFlashSpeed)
	{
		lastTime = thisTime;
		textFlashOn = !textFlashOn;
	}
	int toRemove = 0;
	float thisTime = glfwGetTime();
	for (int i = (int)chatLog.size() - 1; i >= 0; i--)
	{
		GFX::getOverlay()->drawStringBG(chatLog.at(i), 5, Window::getWindow()->getHeight() - 65 - (chatLog.size() - i) * 30, 30, 1, 1, 1, 1, 0, -2, 0, 0, 0, 0, 0, 0.3);
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
	if (isChatting)
	{
		GFX::getOverlay()->fillRect(5, Window::getWindow()->getHeight() - 35, Window::getWindow()->getWidth() - 10, 30, 0, 0, 0, 0.8);
		std::string outmsg = "";
		outmsg += currentMessage;
		if (textFlashOn)
		{
			outmsg += "_";
		}
		if (commandError)
		{
			GFX::getOverlay()->drawString(outmsg, 7, Window::getWindow()->getHeight() - 32.5, 30, 1, 0, 0, 1);
		}
		else
		{
			GFX::getOverlay()->drawString(outmsg, 7, Window::getWindow()->getHeight() - 32.5, 30, 1, 1, 1, 1);
		}
		
		if (currentMessage.size() > 0 && currentMessage.at(0) == '/')
		{
			int widestArgument = 0;
			int width = 0;
			std::string cmd = currentMessage.substr(1, currentMessage.length());
			int i = 0;
			int j = currentMessage.find_last_of(' ');
			if (j <= 0)
			{
				j = 1;
			}
			else
			{
				j += 1;
			}
			bool argumentComplete = false;

			if (chatChanged)
			{
				refreshSuggestions();
				chatChanged = false;
			}

			std::string currentArgument = currentMessage.substr(j, currentMessage.length());

			int entries = 0;
			bool midArgument = (currentArgument.length() != 0);

			std::vector<CommandSuggestion> suggestions;
			for (i = 0; i < suggestionList.size(); i++)
			{
				if (suggestionList.at(i).name.rfind(currentArgument, 0) == 0 ||
					(midArgument && suggestionList.at(i).index != Command::Argument::SUGGESTION
						&& suggestionList.at(i).index != Command::Argument::LITERAL &&
						Command::validArgument(currentArgument, suggestionList.at(i).index)))
				{
					entries++;
					if (suggestionList.at(i).name.length() == currentArgument.length())
					{
						argumentComplete = true;
					}
					int w;
					if (suggestionList.at(i).index == Command::Argument::LITERAL)
					{
						w = GFX::getOverlay()->stringWidth(suggestionList.at(i).name, 30);
					}
					else if (suggestionList.at(i).index == Command::Argument::SUGGESTION)
					{
						w = GFX::getOverlay()->stringWidth(suggestionList.at(i).name, 30);
					}
					else
					{
						w = GFX::getOverlay()->stringWidth(suggestionList.at(i).name
							+ ": " + Command::argumentTypeStrings[suggestionList.at(i).index], 30);
					}
					if (w > widestArgument)
					{
						widestArgument = w;
					}
				}
			}
			if (currentMessage.size() <= 1)
			{
				argumentComplete = false;
			}
			argumentComplete = false;
			if (!argumentComplete || commandError)
			{
				width = GFX::getOverlay()->stringWidth(currentMessage.substr(0, currentSuggestionIndex), 30);
				if (!commandError)
				{
					GFX::getOverlay()->fillRect(width + 5, Window::getWindow()->getHeight() - 35 - 1, widestArgument, -30 * entries, 0, 0, 0, 0.8);
				}
				i = 0;
				int entriesIndex = 1;
				std::string currentSuggestion;
				std::vector<std::string> expectedValues;
				for (i = suggestionList.size() - 1; i >= 0; i--)
				{
					if (suggestionList.at(i).name.rfind(currentArgument, 0) == 0 || commandError ||
						(midArgument && suggestionList.at(i).index != Command::Argument::SUGGESTION
							&& suggestionList.at(i).index != Command::Argument::LITERAL &&
							Command::validArgument(currentArgument, suggestionList.at(i).index)))
					{
						currentSuggestion = suggestionList.at(i).name;
						if (suggestionList.at(i).index == Command::Argument::SUGGESTION)
						{
							if (!commandError)
							{
								currentSuggestion = suggestionList.at(i).name;
								if (autofillIndex == i)
								{
									GFX::getOverlay()->drawString(currentSuggestion, 5 + width, Window::getWindow()->getHeight() - 32.5 - entriesIndex * 30 - 1, 30, 0.7, 0.7, 0.0, 1);
								}
								else
								{
									GFX::getOverlay()->drawString(currentSuggestion, 5 + width, Window::getWindow()->getHeight() - 32.5 - entriesIndex * 30 - 1, 30, 0.7, 0.7, 0.7, 1);
								}
							}
						}
						else if (suggestionList.at(i).index != Command::Argument::LITERAL)
						{
							currentSuggestion = suggestionList.at(i).name
								+ ": " + Command::argumentTypeStrings[suggestionList.at(i).index];
						}
						if (suggestionList.at(i).index != Command::Argument::SUGGESTION)
						{
							if (commandError)
							{
								currentSuggestion = Command::argumentTypeStrings[suggestionList.at(i).index] + " <" + suggestionList.at(i).name + ">";
								expectedValues.emplace_back(currentSuggestion);
							}
							else
							{
								if (autofillIndex == i)
								{
									GFX::getOverlay()->drawString(currentSuggestion, 5 + width, Window::getWindow()->getHeight() - 32.5 - entriesIndex * 30 - 1, 30, 1, 1, 0, 1);
								}
								else
								{
									GFX::getOverlay()->drawString(currentSuggestion, 5 + width, Window::getWindow()->getHeight() - 32.5 - entriesIndex * 30 - 1, 30, 1, 1, 1, 1);
								}
							}
						}
						if (!commandError && (suggestionList.at(i).index == Command::Argument::SUGGESTION
							|| suggestionList.at(i).index == Command::Argument::LITERAL))
						{
							
							GFX::getOverlay()->drawString(
								currentSuggestionMsg, width - 5, Window::getWindow()->getHeight() - 32.5, 30, 1, 0, 1, 0.5);
						}

						entriesIndex++;
					}
				}
				if (expectedValues.size() > 0)
				{
					std::string error = "Expected ";
					for (int i = expectedValues.size() - 1; i >= 0; i--)
					{
						if (i == 1)
						{
							error += expectedValues.at(i) + " or ";
						}
						else if (i == 0)
						{
							error += expectedValues.at(i);
						}
						else
						{
							error += expectedValues.at(i) + ", ";
						}
					}
					GFX::getOverlay()->fillRect(width + 5, Window::getWindow()->getHeight() - 35 - 1,
						GFX::getOverlay()->stringWidth(error, 30), -30 * 1, 0, 0, 0, 0.8);
					GFX::getOverlay()->drawString(error, 5 + width, Window::getWindow()->getHeight() - 32.5 - 1 * 30 - 1, 30, 1, 0, 0, 1);
				}
				if (suggestionList.size() == 0 && commandError && currentSuggestionIndex <= 2)
				{
					std::string currentSuggestion = "Unknown command";
					GFX::getOverlay()->fillRect(width + 5, Window::getWindow()->getHeight() - 35 - 1,
						GFX::getOverlay()->stringWidth(currentSuggestion, 30), -30 * 1, 0, 0, 0, 0.8);
					GFX::getOverlay()->drawString(currentSuggestion, 5 + width, Window::getWindow()->getHeight() - 32.5 - 30 - 1, 30, 1, 0, 0, 1);
				}
				else if (suggestionList.size() == 0 && commandError)
				{
					std::string currentSuggestion = "Unexpected argument";
					GFX::getOverlay()->fillRect(width + 5, Window::getWindow()->getHeight() - 35 - 1,
						GFX::getOverlay()->stringWidth(currentSuggestion, 30), -30 * 1, 0, 0, 0, 0.8);
					GFX::getOverlay()->drawString(currentSuggestion, 5 + width, Window::getWindow()->getHeight() - 32.5 - 30 - 1, 30, 1, 0, 0, 1);
				}
			}
		}
	}
	if (!f1Lock && Window::getWindow()->getController()->isKeyDown(GLFW_KEY_F1))
	{
		auto t = time(nullptr);
		auto tm = *localtime(&t);
		f1Lock = true;
		std::string outFile = Resource::getInstance().DIR + "//Screenshots//";
		std::stringstream ss;
		ss << Resource::getInstance().DIR + "//Screenshots//";
		//ss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		ss << ".png";
		int save_result = SOIL_save_screenshot
		(
			ss.str().c_str(),
			SOIL_SAVE_TYPE_BMP,
			0, 0, Window::getWindow()->getWidth(), Window::getWindow()->getHeight()
		);
		message(ss.str());
	}
	else if (!Window::getWindow()->getController()->isKeyDown(GLFW_KEY_F1))
	{
		f1Lock = false;
	}
}

void Chat::message(const std::string& msg)
{
	chatLog.emplace_back(msg);
	chatLogTimings.emplace_back(glfwGetTime());
	Log::out("Chat", msg, YELLOW);
}

void Chat::command(const std::string& cmd)
{
	std::vector<std::string> parse = Util::splitString(cmd, " ");
	/*if (parse.size() > 1 && parse.at(0).compare("speed") == 0)
	{
		float speed = std::stof(parse.at(1));
		Log::out("Command", "Speed set to " + std::to_string(speed), RED);
		GFX::getOverlay()->CAM.setSpeedMultiplier(speed);
	}
	if (parse.size() > 0 && parse.at(0).compare("save") == 0)
	{
		Log::out("Command", "Saving...", RED);
		for (auto &c : w->overworld)
		{
			ChunkIO::saveChunk(c.get(), "myworld");
		}
		Log::out("Command", "Save complete.", RED);
	}*/

	if (CommandRegistry::commands.find(parse.at(0)) == CommandRegistry::commands.end())
	{
		message("Command not found!");
	}
	else
	{
		Log::out("Command", cmd);
		CommandRegistry::commands.at(parse.at(0)).run(parse, std::map<std::string, std::vector<std::string>>(), 0);
	}

}
