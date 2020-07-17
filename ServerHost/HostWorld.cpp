#include "pch.h"
#include "HostWorld.h"
#include "M.h"

HostWorld::HostWorld()
{
}


HostWorld::~HostWorld()
{
}
int xx = 0;
int yy = 0;
std::shared_ptr<Chunk> HostWorld::getChunk(int x, int y)
{
	for (auto c : overworld)
	{
		if (c->x == x && c->y == y)
		{
			return c;
		}
	}
	return nullptr;
	/*std::shared_ptr<Chunk> c(new Chunk());
	c->generate();
	c->x = xx - 1;
	c->y = yy - 1;
	xx++;
	if (xx > 6)
	{
		yy++;
		xx = 0;
	}
	if (yy > 6)
	{
		yy = 0;
	}
	return c;*/
}
