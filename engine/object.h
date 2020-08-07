#ifndef OBJECT_H
#define OBJECT_H

#include "common_includes.h"
#include "shader.h"


class EngineObject
{
private:
	GLuint vertex_array_object;
	GLuint vertex_buffer_object;
	GLuint element_buffer_object;
	GLfloat * PtrToDataArray;
	GLuint DataArrayLength;
	GLuint * PtrToEboArray;
	GLuint DataElementLength;

	EngineShader vshader;
	EngineShader fshader;
public:
	EngineObject();
	void CreateObject();


	//set functions
	void SetDataArray(GLfloat * data_arr,GLuint length);
	void SetElementArray(GLuint * data_arr,GLuint length);
	void CreateFShader();
	void CreateFShader();

	//get functions
	GLuint GetVertexArray();
	GLuint GetVertexBuffer();
	GLuint GetElementBuffer();
	GLuint GetVShader();
	GLuint GetFShader();
};


#endif