#include "object.h"


EngineObject::EngineObject()
{
	//initializing vertex array and buffer
	glGenVertexArrays(1, &this->vertex_array_object);
	glGenBuffers(1, &this->vertex_buffer_object);
}

void EngineObject::SetDataArray(GLfloat * data_arr, GLuint length)
{
	this->PtrToDataArray = data_arr;
	this->DataArrayLength = length;
}

void EngineObject::SetElementArray(GLuint * data_arr, GLuint length)
{
	this->PtrToEboArray = data_arr;
	this->DataElementLength = length;
}

void EngineObject::CreateObject()
{
	glBindVertexArray(this->vertex_array_object);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_object);
	glBufferData(GL_ARRAY_BUFFER, this->DataArrayLength, &this->PtrToDataArray, GL_STATIC_DRAW);
}


GLuint EngineObject::GetVertexArray()
{
	return this->vertex_array_object;
}

GLuint EngineObject::GetVertexBuffer()
{
	return this->vertex_buffer_object;
}

GLuint EngineObject::GetElementBuffer()
{
	return this->element_buffer_object;
}

GLuint EngineObject::GetVShaderGLuint()
{
	return vshader.GetShaderGLuint();
}

GLuint EngineObject::GetFShaderGLuint()
{
	return fshader.GetShaderGLuint();
}

EngineShader EngineObject::GetVShaderObj()
{
	return vshader;
}

EngineShader EngineObject::GetFShaderObj()
{
	return fshader;
}

void EngineObject::CreateVShader(const char * filename)
{
	this->vshader = EngineShader(filename, GL_VERTEX_SHADER);
}

void EngineObject::CreateFShader(const char * filename)
{
	this->fshader = EngineShader(filename, GL_FRAGMENT_SHADER);
}

void EngineObject::DrawObject(GLuint count_to_draw)
{
	glBindVertexArray(this->vertex_array_object);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertex_buffer_object);
	glDrawArrays(GL_TRIANGLES, 0, count_to_draw);
}