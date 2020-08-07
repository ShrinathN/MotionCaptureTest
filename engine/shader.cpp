#include "shader.h"

EngineShader::EngineShader()
{
	return;
}

EngineShader::EngineShader(const char * filepath, GLuint shader_type)
{
	this->LoadShaderFromFile(filepath);
	//setting shader type in object
	this->shader_type = shader_type;
}

void EngineShader::SetShaderType(GLint shader_type)
{
	this->shader_type = shader_type;
}

void EngineShader::LoadShaderFromFile(const char * filepath)
{
	//for file length
	unsigned long file_length;
	//file
	FILE *in = NULL;

	//opening file
	in = fopen(filepath, "r");
	//if file could not be opened
	if (in == NULL)
	{
		fprintf(stderr, "Could not open file!\n");
		fflush(stdout);
		fflush(stderr);
		return;
	}
	//getting file length
	fseek(in, 0, SEEK_END);
	file_length = ftell(in) + 1;
	fseek(in, 0, SEEK_SET);

	//allocating memory for the shader and copying shader
	this->ptr_to_shader = (char *)malloc(file_length);
	bzero(this->ptr_to_shader, file_length);
	fread(this->ptr_to_shader, file_length, 1, in);
	fclose(in);
}

void EngineShader::CompileShader()
{
	// printf("%s\n", this->ptr_to_shader);
	//if compilation was successful
	GLint success;
	//length of the compile error / warning log
	GLint log_length;

	this->shader = glCreateShader(this->shader_type);
	glShaderSource(this->shader, 1, &this->ptr_to_shader, NULL);
	glCompileShader(this->shader);
	glGetShaderiv(this->shader, GL_COMPILE_STATUS, &success);
	glGetShaderiv(this->shader, GL_INFO_LOG_LENGTH, &log_length);

	//this means 0 errors or warnings, most likely there will be errors or warnings etc
	if (log_length == 0 && success) return;

	//allocating space for buffer and printing buffer
	char * log_buffer = (char *)malloc(log_length);
	bzero(log_buffer, log_length);
	glGetShaderInfoLog(this->shader, log_length, NULL, log_buffer);
	printf("%s", log_buffer);
}

GLuint EngineShader::GetShaderGLuint()
{
	return this->shader;
}

char * EngineShader::GetPtrToSource()
{
	return this->ptr_to_shader;
}


EngineProgram::EngineProgram()
{
	this->program = glCreateProgram();
}

GLuint EngineProgram::GetProgramGLuint()
{
	return this->program;
}

void EngineProgram::AddShader(EngineShader shader)
{
	glAttachShader(this->GetProgramGLuint(), shader.GetShaderGLuint());
}

void EngineProgram::LinkProgram()
{
	GLint success;
	GLint buffer_length;

	glLinkProgram(this->GetProgramGLuint());
	glGetProgramiv(this->GetProgramGLuint(), GL_COMPILE_STATUS, &success);
	glGetProgramiv(this->GetProgramGLuint(), GL_INFO_LOG_LENGTH, &buffer_length);

	if(buffer_length == 0 && success) return;

	char * buffer_log = (char*)malloc(buffer_length);
	bzero(buffer_log, buffer_length);
	glGetProgramInfoLog(this->GetProgramGLuint(), buffer_length, NULL, buffer_log);
	printf("%s\n", buffer_log);
}

void EngineProgram::UseProgram()
{
	glUseProgram(this->program);
}