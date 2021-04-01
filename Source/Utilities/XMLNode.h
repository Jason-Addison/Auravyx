#pragma once
#include <string>
#include <map>
#include <list>
class XMLNode
{
public:

	std::string getName();

	std::string getData();

	std::string getAttribute(std::string attrib);

	XMLNode getChild(std::string childName);

	XMLNode getChildWithAttribute(std::string childName, std::string attrib, std::string value);

	std::list<XMLNode> getChildren(std::string name);

protected:

	XMLNode();

	XMLNode(std::string name);

	void addAttribute(std::string attrib, std::string value);

	void addChild(XMLNode child);

	void setData(std::string data);

private:

	std::string name;
	std::map<std::string, std::string> attributes;
	std::string data;
	std::map<std::string, std::list<XMLNode>> childNodes;
};

