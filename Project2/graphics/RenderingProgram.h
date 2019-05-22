#pragma once

#include <GL/glew.h>
#include<string>
#include<graphics/Shader.h>

class RenderingProgram
{
public:

	GLuint programID;
	void addShader(Shader shader);
	void linkProgram();
	inline void useProgram() { glUseProgram(programID); };
	inline void stopProgram() { glUseProgram(0); };
	inline void createProgram(){ programID = glCreateProgram(); }
	RenderingProgram();
	~RenderingProgram();
};

