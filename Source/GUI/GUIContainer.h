#pragma once
#include "GUI.h"
#include "GUIContainerData.h"
class GUIContainer : public GUI
{
public:

	float x, y;
	int maxXBound = 0, maxYBound = 0;
	int maxX, maxY = 5;
	float width, height;
	float spacing;

	std::vector<std::shared_ptr<GUIContainerData>> containerInfo;
	GUIContainer(float x, float y, float width, float height);
	GUIContainer();
	~GUIContainer();

	void update();

	void render();

	void setSpacing(float spacing);

	void handleResize();

	void add(std::shared_ptr<GUI> c, float x, float y, float width, float height)
	{
		guis.emplace_back(c);
		containerInfo.emplace_back(std::shared_ptr<GUIContainerData>(new GUIContainerData(c, x, y, width, height)));
	}

	template <typename T> void add(float x, float y, float width, float height)
	{
		std::shared_ptr<T> emptyComponent(new T());
		add(emptyComponent, x, y, width, height);
	}

	void setBounds(float x, float y, float width, float height);

	void setMaxBounds(int width, int height);
};

