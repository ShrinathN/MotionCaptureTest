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
	glBufferData(GL_ARRAY_BUFFER, this->DataArrayLength, this->PtrToDataArray, GL_STATIC_DRAW);
}
