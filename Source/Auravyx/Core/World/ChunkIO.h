#pragma once
#include "Auravyx/Core/World/Chunk.h"
#include <memory>

/**
 * @brief Loading and saving chunks.
*/
class ChunkIO
{
public:

	static void saveArea();

	/**
	 * @brief Saves chunk data to file.
	 * @param chunk to save
	 * @param name file name
	*/
	static void saveChunk(const Chunk* chunk, const std::string& name);

	/**
	 * @brief Returns pointer to Chunk read from file.
	 * @param x chunk coordinate
	 * @param y chunk coordinate
	 * @param z chunk coordinate
	 * @param name string value of file
	 * @return shared_ptr of Chunk that has data but is not loaded
	*/
	static std::shared_ptr<Chunk> readChunk(const int x, const int y, const int z, const std::string& name);

protected:

	ChunkIO() = default;
	virtual ~ChunkIO() = 0;
};

