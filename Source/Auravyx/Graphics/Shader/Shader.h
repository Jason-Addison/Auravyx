#pragma once
#include <string>
#include <map>
#include <GL/glew.h>
#include "Auravyx/Utility/Math/Matrix4f.h"
class Shader
{
public:

	Shader();
	~Shader();
	GLuint shaderProgram;
	void createShader(const std::map<std::string, std::string>& shaders);

	void start();

	void stop();

	static void destroyShaders();

	virtual void loadAllUniformLocations();

protected:

	GLint getUniformLocation(const GLchar *name);

	GLint getUniformLocation(const std::string& name);

	std::string shader = "No shader specified";

	void loadFloat(const GLint location, const float value);

	void loadVec4f(const GLint location, const float x, const float y, const float z, const float w);

	void loadVec3f(const GLint location, const float x, const float y, const float z);

	void loadVec2f(const GLint location, const float x, const float y);

	void loadMatrix4f(const GLint location, const Matrix4f& value);

	void loadTexture(const GLchar* location, const int unit);

	void loadTexture(const std::string& location, const int unit);

	void bindAttribute(const int attribute, const GLchar* variableName);


};

