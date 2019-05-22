#include "RenderingProgram.h"
#include <XPLMUtilities.h>


void RenderingProgram::addShader(Shader shader)
{
	shader.attachShader(programID);

}

void RenderingProgram::linkProgram()
{
	int success;
	char log[1024];
	glLinkProgram(programID);
	glGetProgramiv(programID, GL_LINK_STATUS, &success);

	if (!success) {
		glGetProgramInfoLog(programID, sizeof(log), NULL, log);
		XPLMDebugString("PROGRAM LINKING FAILED: ");
		strcat(log, "\n");
		XPLMDebugString(log);
	}
	else {
		XPLMDebugString("PROGRAM SUCCESSFULLY LINKED AND READY FOR USE\n");
	}
}

RenderingProgram::RenderingProgram()
{
	
}


RenderingProgram::~RenderingProgram()
{
	glDeleteProgram(programID);
}
