#include "stdafx.h"
#include "Shader/Shader.h"
#include "Utilities/Util.h"
#include "Utilities/Log.h"
#include "Utilities/Resource.h"
#include <iostream>
Shader::Shader()
{

}


Shader::~Shader()
{
}

static std::vector<Shader*> shaderPtrs;

GLint Shader::getUniformLocation(GLchar *name)
{
	GLint location = glGetUniformLocation(shaderProgram, name);
	return location;
}

GLint Shader::getUniformLocation(std::string name)
{
	return getUniformLocation((GLchar*) name.c_str());
}

void Shader::loadFloat(GLint location, float value)
{
	glUniform1f(location, value);
}

void Shader::loadVec4f(GLint location, float x, float y, float z, float w)
{
	glUniform4f(location, x, y, z, w);
}

void Shader::loadVec3f(GLint location, float x, float y, float z)
{
	glUniform3f(location, x, y, z);
}

void Shader::loadVec2f(GLint location, float x, float y)
{
	glUniform2f(location, x, y);
}

void Shader::loadMatrix4f(GLint location, Matrix4f value)
{
	GLfloat mat[] = { value.m00, value.m01, value.m02, value.m03,
		value.m10, value.m11, value.m12, value.m13,
		value.m20, value.m21, value.m22, value.m23,
		value.m30, value.m31, value.m32, value.m33 };

	/*GLfloat mat[] = { value.m00, value.m10, value.m20, value.m30,
	value.m01, value.m11, value.m21, value.m31,
	value.m02, value.m12, value.m22, value.m32,
	value.m03, value.m13, value.m23, value.m33 };*/
	glUniformMatrix4fv(location, 1, GL_FALSE, mat);
}

void outputErrorMessage(std::string shader, std::string fileData, std::string error, std::string shaderType)
{
	std::vector<Vec3f> colours;

	std::vector<std::string> source = Util::splitString(error, '\n');
	std::vector<std::string> lines = Util::splitString(fileData, '\n');
	std::vector<std::string> problemLines;

	int lastY = -1;

	problemLines.emplace_back("Error in " + shaderType + " shader '" + shader + "'");
	colours.emplace_back(Vec3f(1, 1, 1));

	problemLines.emplace_back("");
	colours.emplace_back(Vec3f(1, 1, 1));

	for (int i = 0; i < source.size(); i++)
	{
		problemLines.emplace_back(source.at(i));
		colours.emplace_back(Vec3f(1, 1, 1));
	}

	for (int i = 0; i < source.size() - 1; i++)
	{
		std::vector<std::string> splitA = Util::splitString(source.at(i), '(');
		std::vector<std::string> splitB = Util::splitString(splitA.at(1), ')');

		int x = stoi(splitA.at(0));
		int y = stoi(splitB.at(0));

		if (y != lastY)
		{
			for (int j = y - 3; j < y + 3; j++)
			{
				if (j > 0 && j < lines.size())
				{
					colours.emplace_back(Vec3f(1, 0, 0));
					std::string o = std::to_string(j) + " : " + lines.at(j);
					problemLines.emplace_back(o);
					lastY = y;
				}
			}
			problemLines.emplace_back("");
			colours.emplace_back(Vec3f(1, 1, 1));
		}
	}
	problemLines.emplace_back("Starting anyways");
	colours.emplace_back(Vec3f(1, 1, 1));

	Resource::getResources()->printLoadingMessage(problemLines, 15, colours);
}

void Shader::createShader(std::map<std::string, std::string> shaders)
{
	std::string vert;
	std::string frag;
	try
	{
		vert = shaders.at(shader + ".vert");
	}
	catch (std::out_of_range e)
	{
		Log::out("[OpenGL] Error loading vertex shader : " + shader);
	}
	try
	{
		frag = shaders.at(shader + ".frag");
	}
	catch (std::out_of_range e)
	{
		Log::out("[OpenGL] Error loading fragment shader : " + shader);
	}
	const char* cVert = vert.c_str();
	const char* cFrag = frag.c_str();
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &cVert, 0);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &cFrag, 0);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glValidateProgram(shaderProgram);

	GLint success = 0;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		Log::out("GLSL Vertex Shader Error", "Failed to compile! " + vert, RED, RED);
		GLint logSize = 0;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
		int i = logSize;
		GLchar *error = new GLchar[logSize];
		glGetShaderInfoLog(vertexShader, logSize, NULL, error);
		const char* logC = error;
		std::string log(error);

		outputErrorMessage(shader, frag, log, "fragment");

		Log::outNNL(log + "\n", WHITE);
		delete[] error;
	}

	success = 0;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		Log::out("GLSL Fragment Shader Error", "Failed to compile! " + frag, RED, RED);
		GLint logSize = 0;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
		int i = logSize;
		GLchar *error = new GLchar[logSize];
		glGetShaderInfoLog(fragmentShader, logSize, NULL, error);
		const char* logC = error;
		std::string log(error);
		delete[] error;

		outputErrorMessage(shader, frag, log, "fragment");
		
		Log::outNNL(log + "\n", WHITE);
	}
	shaderPtrs.emplace_back(this);
	start();
	loadAllUniformLocations();
	stop();
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::start()
{
	glUseProgram(shaderProgram);
}

void Shader::stop()
{
	glUseProgram(0);
}

void Shader::destroyShaders()
{
	for (Shader* i : shaderPtrs)
	{
		delete i;
	}
	shaderPtrs.clear();
	std::vector<Shader*>().swap(shaderPtrs);
}

void Shader::loadAllUniformLocations()
{
}

void Shader::loadTexture(GLchar* location, int unit)
{
	glUniform1i(getUniformLocation(location), unit);
}
void Shader::loadTexture(std::string location, int unit)
{
	loadTexture((GLchar*) location.c_str(), unit);
}
void Shader::bindAttribute(int attribute, GLchar* variableName)
{
	glBindAttribLocation(shaderProgram, attribute, variableName);
}