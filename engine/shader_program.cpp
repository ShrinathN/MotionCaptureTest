#include "shader_program.h"



EngineProgram::EngineProgram()
{
	this->program = glCreateProgram();
}

GLuint * EngineProgram::GetProgramGLuint()
{
	return &this->program;
}

void EngineProgram::AddShader(GLuint *shader)
{
	glAttachShader(this->program, *shader);
}

void EngineProgram::LinkProgram()
{
	GLint success;
	GLint buffer_length;

	glLinkProgram(*this->GetProgramGLuint());
	glGetProgramiv(*this->GetProgramGLuint(), GL_COMPILE_STATUS, &success);
	glGetProgramiv(*this->GetProgramGLuint(), GL_INFO_LOG_LENGTH, &buffer_length);

	if(buffer_length == 1 && success) return;

	char * buffer_log = (char*)malloc(buffer_length);
	bzero(buffer_log, buffer_length);
	glGetProgramInfoLog(*this->GetProgramGLuint(), buffer_length, NULL, buffer_log);
	printf("%s\n", buffer_log);
	fflush(stdout);
	free(buffer_log);
}

void EngineProgram::UseProgram()
{
	glUseProgram(this->program);
}