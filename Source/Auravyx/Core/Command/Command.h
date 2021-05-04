#pragma once
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>
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
		DOUBLE,
		STRING_OPTION,
		SUGGESTION
	};

	const static std::string argumentTypeStrings[];

	Argument argumentType;
	std::string argumentValue = "";

	std::vector<Command> subCommands;
	std::function<int(std::map<std::string, std::vector<std::string>>&)> otherwiseFunction;
	std::shared_ptr<std::vector<std::string>> suggestions;
	bool limitToSuggestions = false;

	Command& condition(Command subCmd);
	Command& otherwise(std::function<int(std::map<std::string, std::vector<std::string>>&)> function);
	Command& limits(std::shared_ptr<std::vector<std::string>> suggestions);
	Command& suggests(std::shared_ptr<std::vector<std::string>> suggestions);

	static Command literal(std::string s);

	static Command literals(std::vector<std::string> literals);

	static Command string();

	static Command selectedString();

	static Command integer();

	static Command doubleArgument();

	static Command argument(std::string argName, Command subCmd);

	Command& set(std::string commandName);

	static int getInteger(std::string argName, std::map<std::string, std::vector<std::string>>& args);

	static double getDouble(std::string argName, std::map<std::string, std::vector<std::string>>& args);

	static bool validArgument(std::string s, int arg);

	bool validArgument(std::string s);

	bool run(std::vector<std::string> arguments, std::map<std::string, std::vector<std::string>>& argumentMap, int i);

	virtual void load();
};