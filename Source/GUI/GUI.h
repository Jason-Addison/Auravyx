#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include "Util.h"
#include <functional>
class GUI
{
public:
	GUI();
	~GUI();

	bool ra = false; //Right aligned

	std::vector<std::shared_ptr<GUI>> guis;

	std::shared_ptr<GUI> parent;

	bool disableResizeUpdate = false;

	bool visible = true;

	std::function<void()> updateEvent = []() {};
	std::function<void()> renderEvent = []() {};
	std::function<void()> resizeEvent = []() {};

	void refresh();


	virtual void handleResize();

	virtual void update();

	virtual void render();

	virtual void setBounds(float x, float y, float width, float height);

	template <typename T> std::shared_ptr<T> gettt()
	{
		/*std::type_index index(typeid(T));
		if (guis.count(index) != 0)
		{
			return std::static_pointer_cast<T>(guis[index]);
		}
		else
		{
			Util::log("GUI not found! " + std::to_string(guis.size()) + "   " + std::to_string(guis.count(index)));
			return std::shared_ptr<T>();
		}*/
		return NULL;
	}

	template <typename T> bool contains()
	{
		std::shared_ptr<T> emptyComponent(new T());
		if (std::find(guis.begin(), guis.end(), emptyComponent) != guis.end())
		{
			return true;
		}
		return false;
	}

	void add(std::shared_ptr<GUI> c)
	{
		guis.emplace_back(c);
		notifyNewGUI();
	}

	template <typename T> void add()
	{
		std::shared_ptr<T> emptyComponent(new T());
		add(emptyComponent);
	}

	virtual void notifyNewGUI()
	{

	}

	void setUpdateEvent(std::function<void()> updateEvent)
	{
		this->updateEvent = updateEvent;
	}
	void setRenderEvent(std::function<void()> renderEvent)
	{
		this->renderEvent = renderEvent;
	}
};

