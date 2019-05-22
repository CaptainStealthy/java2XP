#pragma once
#include <GL/glew.h>
#include <string>


class Shader
{
public:
	static enum shader_type { shader_vertex, shader_fragment };
	GLint shaderID;
	const char * shaderCode;
	shader_type type;
	std::string * shaderPath;

	void loadShader(std::string pathToFile, shader_type sType);
	inline void attachShader(GLuint programID) { glAttachShader(programID, shaderID); };

	Shader();
	~Shader();
};

