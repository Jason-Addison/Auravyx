#pragma once
#include "MSVC/pch.h"
#include "Auravyx/Utility/Math/Vec3f.h"
#include "Auravyx/Utility/Math/Matrix4f.h"
#include <vector>

/**
 * @brief Extended math class to provide additional functionality to C / C++'s math libraries.
*/
class M
{
public:

	/**
	 * @brief Convert degrees to radians.
	 * @param degrees degree to convert to radians
	 * @return radian value of 'degrees'
	*/
	static float toRadians(const float degrees);

	/**
	 * @brief Generate random integer between two values inclusive.
	 * @param min minimum number
	 * @param max maximum number
	 * @return randomly generated integer between 'min' and 'max' inclusive
	*/
	static int randomInt(const int min, const int max);

	/**
	 * @brief Generate random float between two values inclusive.
	 * @param min minimum number
	 * @param max maximum number
	 * @return randomly generated float between 'min' and 'max' inclusive
	*/
	static float randomFloat(const float min, const float max);

	/**
	 * @brief Distance between two points.
	 * @details sdajik wa aw aw
	 * @param a first point
	 * @param b second point
	 * @return distance between points 'a' and 'b'
	*/
	static float distance(const Vec3f& a, const Vec3f& b);

	/**
	 * @brief Create transformation matrix.
	 * @param x position
	 * @param y position
	 * @param z position
	 * @param width scale on x-axis
	 * @param height scale on y-axis
	 * @param depth scale on z-axis
	 * @param xRot rotation on x-axis in degrees
	 * @param yRot rotation on y-axis in degrees
	 * @param zRot rotation on z-axis in degress
	 * @return transformation matrix
	*/
	static Matrix4f createTransformationMatrix(const float x, const float y, const float z, const float width, const float height, const float depth, const float xRot, const float yRot, const float zRot);

	/**
	 * @brief Magnitude of 3D vector.
	 * @param x vector value
	 * @param y vector value
	 * @param z vector value
	 * @return magnitude of vector
	*/
	static float mag(const float x, const float y, const float z);

	/**
	 * @brief Normalize vector elements between -1 and 1.
	 * @param v vector to normalize
	 * @return normalized vector
	*/
	static Vec3f normalize(const Vec3f& v);

	/**
	 * @brief Convert integer into byte representation, stored by least significant digit first.
	 * @param x integer to convert
	 * @return vector containing integer in bytes
	*/
	static std::vector<char> intToBytes(const int x);

	/**
	 * @brief Convert byte array to an integer, stored by least significant digit first.
	 * @param b bytes to convert
	 * @return integer value of bytes
	*/
	static int bytesToInt(const char* b);

	/**
	 * @brief Round to nearest point of precision.
	 * @param x value to round
	 * @param precision value of precission
	 * @return rounded value
	*/
	static double roundToDecimal(const double x, const int precision);

protected:

	/**
	 * @brief Base constructor. Do not use.
	*/
	M();
	/**
	 * @brief Pure virtual destructor. Do not use.
	 * @pure
	*/
	virtual ~M() = 0;
};

