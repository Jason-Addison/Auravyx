#include "State.h"
#include <stdio.h>

State::State()
{
}

State::~State()
{
}

void State::update()
{
	printf("[Error] Calling base State update function\n");
}

void State::render()
{
	printf("[Error] Calling base State rgender function\n");
}

void State::start()
{
}

void State::stop()
{
}
