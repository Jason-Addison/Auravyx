#pragma once
#include <string>
#include <map>
#include <list>
class XMLNode
{
public:

	std::string getName();

	std::string getData();

	std::string getAttribute(const std::string& attrib);

	XMLNode getChild(const std::string& childName);

	XMLNode getChildWithAttribute(const std::string& childName, const std::string& attrib, const std::string& value);

	std::list<XMLNode> getChildren(const std::string& name);

protected:

	XMLNode();

	XMLNode(const std::string& name);

	void addAttribute(const std::string& attrib, const std::string& value);

	void addChild(const XMLNode& child);

	void setData(const std::string& data);

private:

	std::string name;
	std::map<std::string, std::string> attributes;
	std::string data;
	std::map<std::string, std::list<XMLNode>> childNodes;
};

