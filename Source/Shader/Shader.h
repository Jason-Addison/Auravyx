#pragma once
#include <string>
#include <map>
#include <GL/glew.h>
#include "Matrix4f.h"
class Shader
{
public:

	Shader();
	~Shader();
	GLuint shaderProgram;
	void createShader(std::map<std::string, std::string> shaders);

	void start();

	void stop();

	static void destroyShaders();

	virtual void loadAllUniformLocations();

protected:

	GLint getUniformLocation(GLchar *name);

	GLint getUniformLocation(std::string name);

	std::string shader = "No shader specified";

	void loadFloat(GLint location, float value);

	void loadVec4f(GLint location, float x, float y, float z, float w);

	void loadVec3f(GLint location, float x, float y, float z);

	void loadVec2f(GLint location, float x, float y);

	void loadMatrix4f(GLint location, Matrix4f value);

	void loadTexture(GLchar* location, int unit);

	void loadTexture(std::string location, int unit);

	void bindAttribute(int attribute, GLchar* variableName);


};

