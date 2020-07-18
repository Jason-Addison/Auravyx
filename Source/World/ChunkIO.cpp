#include "ChunkIO.h"
#include "Resource.h"
#include "Log.h"
ChunkIO::ChunkIO()
{
}


ChunkIO::~ChunkIO()
{

}

bool verboseChunkIO = false;

void ChunkIO::saveChunk(std::shared_ptr<Chunk> chunk)
{
	std::string dir = Resource::DIR + "\\Saves\\World\\c" +
		std::to_string(chunk->x) + "." + std::to_string(chunk->y) + "." + std::to_string(chunk->z) + ".voxc";

	FILE* chunkOutput = fopen(dir.c_str(), "wb");
	if (!chunkOutput)
	{
		Log::out("ChunkIO", "Chunk error : " + std::string(strerror(errno)), RED, RED);
	}
	fwrite(&chunk->x, sizeof(int), 1, chunkOutput);
	fwrite(&chunk->y, sizeof(int), 1, chunkOutput);
	fwrite(&chunk->z, sizeof(int), 1, chunkOutput);
	fwrite(&chunk->emptyChunk, sizeof(char), 1, chunkOutput);
	if (!chunk->emptyChunk)
	{
		fwrite(chunk->density, sizeof(short), 2097152, chunkOutput);
	}
	fclose(chunkOutput);

	if (verboseChunkIO)
	{
		Log::out("ChunkIO", "Saved : " + std::to_string(chunk->x) + " " + std::to_string(chunk->y) +
			" " + std::to_string(chunk->z), DARK_BLUE);
	}

}

std::shared_ptr<Chunk> ChunkIO::readChunk(int x, int y, int z)
{
	std::shared_ptr<Chunk> chunk(new Chunk);

	std::string dir = Resource::DIR + "\\Saves\\World\\c" +
		std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".voxc";

	FILE* chunkInput = fopen(dir.c_str(), "rb");
	if (!chunkInput)
	{
		Log::out("ChunkIO", "Chunk error : " + std::string(strerror(errno)), RED, RED);
		return nullptr;
	}

	int i = 0;
	fread(&chunk->x, sizeof(int), 1, chunkInput);
	fread(&chunk->y, sizeof(int), 1, chunkInput);
	fread(&chunk->z, sizeof(int), 1, chunkInput);
	fread(&chunk->emptyChunk, sizeof(char), 1, chunkInput);
	if (!chunk->emptyChunk)
	{
		fread(chunk->density, sizeof(short), 2097152, chunkInput);
	}
	chunk->x = x;
	chunk->y = y;
	chunk->z = z;
	if (verboseChunkIO)
	{
		Log::out("ChunkIO", "Loaded : " + std::to_string(x) + " " + std::to_string(y) + " " + std::to_string(z), DARK_BLUE);
	}
	fclose(chunkInput);
	return chunk;
}
