#include "Shader.h"
#include <string>
#include <GL/glew.h>
#include <fstream>
#include <globalSingletons/GlobalStore.h>
#include <sstream>
#include <XPLMUtilities.h>

static GlobalStore *store = GlobalStore::getInstance();

void Shader::loadShader(std::string pathToFile, shader_type sType)
{
	//FIRST WE LOAD THE CODE FOR THE SHADER
	std::ifstream shaderFile(store->getProperty("dllDirectory") + "xp2Java-shaders/" + pathToFile);
	std::stringstream fileStream;
	fileStream << shaderFile.rdbuf();

	shaderPath = &pathToFile;

	std::string code = fileStream.str();

	shaderCode = code.c_str();

	GLchar * codeForGl[1];
	codeForGl[0] = (GLchar*)code.c_str();

	if (sType == shader_vertex) {
		shaderID = glCreateShader(GL_VERTEX_SHADER);
	}
	else {
		shaderID = glCreateShader(GL_FRAGMENT_SHADER);
	}
		glShaderSource(shaderID, 1, codeForGl, NULL);

		glCompileShader(shaderID);

		int result;
		char log[1024];

		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

		if (!result) {
			glGetShaderInfoLog(shaderID, sizeof(log), NULL, log);
			strcat(log, "\n");
			XPLMDebugString("SHADER COMPILATION FAILED "); XPLMDebugString(log); 
		}
		else {
			XPLMDebugString("SHADER SUCCESFULLY CREATED!\n");
		}
		
}
	


Shader::Shader()
{
}


Shader::~Shader()
{
	glDeleteShader(shaderID);
}
