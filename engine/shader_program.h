#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "common_includes.h"

class EngineProgram {

private:
	GLuint program;

public:
	EngineProgram();

	//get functions
	GLuint * GetProgramGLuint();

	//set functions
	void AddShader(GLuint *shader);
	void LinkProgram();

	void UseProgram();

};


#endif