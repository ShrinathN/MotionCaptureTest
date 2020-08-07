#include "shader.h"

EngineShader::EngineShader(const char * filepath, GLuint shader_type)
{
	this->shader_type = shader_type;

	unsigned long file_length;
	FILE *in = NULL;
	in = fopen(filepath, "r");
	if (in == NULL) return;

	fseek(in, 0, SEEK_END);
	file_length = ftell(in);
	fseek(in, 0, SEEK_SET);
	ptr_to_shader = (char *)malloc(file_length);
	bzero(ptr_to_shader, file_length);
	fread(ptr_to_shader, file_length, 1, in);
	fclose(in);
}

void EngineShader::CompileShader()
{
	int success;

	this->shader = glCreateShader(this->shader_type);
	glShaderSource(this->shader, 1, &this->ptr_to_shader, NULL);
	glCompileShader(this->shader);
	glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
	
}