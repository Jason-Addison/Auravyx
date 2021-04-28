#include "Auravyx/Core/World/ChunkIO.h"
#include "Auravyx/Utility/IO/Resource.h"
#include "Logger/Log.h"
#include <iostream>
#include <filesystem>
bool verboseChunkIO = false;

void ChunkIO::saveArea()
{
	std::string dir = Resource::getInstance().DIR + "/Saves/m.txt";

	FILE* chunkOutput = fopen(dir.c_str(), "rb+");
	if (!chunkOutput)
	{
		//Log::out("ChunkIO", "Chunk error : " + std::string(strerror(errno)), RED, RED);
	}
	unsigned char chunkCount = 0;
	fwrite(&chunkCount, 1, 1, chunkOutput);
	unsigned char noChunk = 255;
	int x = 3;
	int y = 3;
	int z = 3;
	for (int i = 0; i < 64; i++)
	{
		fwrite(&noChunk, 1, 1, chunkOutput);
	}
	char c = x + 4 * (y * 4 + z);
	fseek(chunkOutput, c + 1, SEEK_SET);
	fwrite(&c, 1, 1, chunkOutput);

	//std::cout << (c / 16) << " " << (c % 16) << "\n";

	fclose(chunkOutput);
}

void ChunkIO::saveChunk(const Chunk* chunk, const std::string& name)
{
	std::string dir = Resource::getInstance().DIR + "/Saves/" + name + "/data/c" +
		std::to_string(chunk->x) + "." + std::to_string(chunk->y) + "." + std::to_string(chunk->z) + ".voxc";

	FILE* chunkOutput = fopen(dir.c_str(), "wb");
	if (!chunkOutput)
	{
		//Log::out("ChunkIO", "Chunk error : " + std::string(strerror(errno)), RED, RED);
	}
	fwrite(&chunk->x, sizeof(int), 1, chunkOutput);
	fwrite(&chunk->y, sizeof(int), 1, chunkOutput);
	fwrite(&chunk->z, sizeof(int), 1, chunkOutput);
	fwrite(&chunk->emptyChunk, sizeof(char), 1, chunkOutput);
	if (!chunk->emptyChunk)
	{
		fwrite(chunk->density, sizeof(short), 262144, chunkOutput);
	}
	fclose(chunkOutput);

	if (verboseChunkIO)
	{
		Log::out("ChunkIO", "Saved : " + std::to_string(chunk->x) + " " + std::to_string(chunk->y) +
			" " + std::to_string(chunk->z), DARK_BLUE);
	}

}

std::shared_ptr<Chunk> ChunkIO::readChunk(const int x, const int y, const int z, const std::string& name)
{
	std::shared_ptr<Chunk> chunk(new Chunk());

	std::string dir = Resource::getInstance().DIR + "/Saves/" + name + "/data/c" +
		std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".voxc";

	FILE* chunkInput = fopen(dir.c_str(), "rb");
	if (!chunkInput)
	{
		//Log::out("ChunkIO", "Chunk error : " + std::string(strerror(errno)), RED, RED);
		return nullptr;
	}

	int i = 0;
	fread(&chunk->x, sizeof(int), 1, chunkInput);
	fread(&chunk->y, sizeof(int), 1, chunkInput);
	fread(&chunk->z, sizeof(int), 1, chunkInput);
	fread(&chunk->emptyChunk, sizeof(char), 1, chunkInput);
	if (!chunk->emptyChunk)
	{
		fread(chunk->density, sizeof(short), 262144, chunkInput);
	}
	else
	{
		std::fill(chunk->density, chunk->density + 262144, 0);
	}
	chunk->x = x;
	chunk->y = y;
	chunk->z = z;
	if (verboseChunkIO)
	{
		Log::out("ChunkIO", "Loaded : " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z), DARK_BLUE);
	}
	fclose(chunkInput);
	chunk->dataLoaded = true;
	return chunk;
}