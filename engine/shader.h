#ifndef SHADER_H
#define SHADER_H

#include "common_includes.h"

class EngineShader {
private:
	GLuint shader;
	char * ptr_to_shader;
	GLuint shader_type;
public:
	EngineShader(const char * filepath, GLuint shader_type);
	void CompileShader();
};

#endif