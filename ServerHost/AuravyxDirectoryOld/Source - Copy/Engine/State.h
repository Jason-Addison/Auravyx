#pragma once
class State
{
public:

	State();
	~State();

	virtual void update();

	virtual void render();

	virtual void start();

	virtual void stop();
};