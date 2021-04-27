#include "XMLNode.h"

std::string XMLNode::getName()
{
	return name;
}

std::string XMLNode::getData()
{
	return data;
}

std::string XMLNode::getAttribute(const std::string& attrib)
{
	if (attributes.size() > 0)
	{
		return attributes.at(attrib);
	}
	else
	{
		return std::string();
	}
}

XMLNode XMLNode::getChild(const std::string& childName)
{
	if (childNodes.size() > 0)
	{
		std::list<XMLNode> nodes = childNodes.at(childName);
		if (nodes.size() > 0)
		{
			return nodes.front();
		}
	}
	return XMLNode();
}

XMLNode XMLNode::getChildWithAttribute(const std::string& childName, const std::string& attrib, const std::string& value)
{
	std::list<XMLNode> children = getChildren(childName);
	if (children.size() > 0)
	{
		return XMLNode();
	}
	for (auto c : children)
	{
		std::string val = c.getAttribute(attrib);
		if (value.compare(val) == 0)
		{
			return c;
		}
	}
}

std::list<XMLNode> XMLNode::getChildren(const std::string& name)
{
	if (childNodes.size() > 0)
	{
		if (childNodes.at(name).size() > 0)
		{
			return childNodes.at(name);
		}
	}
	return std::list<XMLNode>();
}

XMLNode::XMLNode()
{
}

XMLNode::XMLNode(const std::string& name)
{
	this->name = name;
}

void XMLNode::addAttribute(const std::string& attrib, const std::string& value)
{
	attributes.emplace(attrib, value);
}

void XMLNode::addChild(const XMLNode& child)
{
	if (childNodes.at(child.name).size() == 0)
	{
		childNodes.emplace(child.name, childNodes.at(child.name));
	}
	childNodes.at(child.name).emplace_back(child);
}

void XMLNode::setData(const std::string& data)
{
	this->data = data;
}
