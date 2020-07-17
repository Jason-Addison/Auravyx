#include "pch.h"
#include "stdafx.h"
#include "Chunk.h"
#include "M.h"
#include "Log.h"
#include <sstream>
Chunk::Chunk()
{
}


Chunk::~Chunk()
{
}

void Chunk::generate()
{
	for (int i = 0; i < size; i++)
	{
		int e = M::randomInt(0, 100);
		int d = M::randomInt(0, 100);
		if (e < 90)
		{
			tiles[i].tile = 1;
		}
		else if (e < 97)
		{
			tiles[i].tile = 3;
		}
		else
		{
			tiles[i].tile = 2;
		}

		tiles[i].plop = 0;
		if (d < 4)
		{
			//tiles[i].plop = 4;
		}
		else if (d < 6)
		{
			tiles[i].plop = 5;
			tiles[i].tile = 3;
		}
		//if (i / chunkSize == 3)
		{
			//tiles[i].plop = 6;
		}
	}
}

void Chunk::setTile(int x, int y, int id)
{
	//std::wstringstream ws;
	//ws << "|" << std::to_string(x).c_str() << " " << std::to_string(y).c_str();
	//Log::out(ws);
	if (x >= 0 && y >= 0 && x < 16 && y < 16)
	{
		tiles[y * 16 + x].plop = id;
	}
	//std::cout << x << "   " << y << "\n";
}
