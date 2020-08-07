#ifndef SHADER_H
#define SHADER_H

#include "common_includes.h"

class EngineShader {
private:
	GLuint shader;
	char * ptr_to_shader;
	GLuint shader_type;
public:
	EngineShader();
	EngineShader(const char * filepath, GLuint shader_type);
	void CompileShader();

	//set functions
	void LoadShaderFromFile(const char * filepath);
	void SetShaderType(GLint shader_type);

	//get functions
	GLuint GetShaderGLuint();
	char * GetPtrToSource();
};

#endif