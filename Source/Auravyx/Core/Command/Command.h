#pragma once
#include <string>
#include <functional>
#include <map>
class Command
{
public:

	Command();

	enum Argument
	{
		LITERAL,
		STRING,
		INTEGER,
		FLOAT,
		DOUBLE
	};

	const static std::string argumentTypeStrings[];

	Argument argumentType;
	std::string argumentValue = "";

	std::vector<Command> subCommands;
	std::function<int(std::map<std::string, std::vector<std::string>>&)> otherwiseFunction;

	Command& condition(Command subCmd);
	Command& otherwise(std::function<int(std::map<std::string, std::vector<std::string>>&)> function);

	static Command literal(std::string s);

	static Command string(std::string s);

	static Command integer();

	static Command doubleArgument();

	static Command argument(std::string argName, Command subCmd);

	Command& set(std::string commandName);

	static int getInteger(std::string argName, std::map<std::string, std::vector<std::string>>& args);

	static double getDouble(std::string argName, std::map<std::string, std::vector<std::string>>& args);

	bool validArgument(std::string s);

	bool run(std::vector<std::string> arguments, std::map<std::string, std::vector<std::string>>& argumentMap, int i);
};