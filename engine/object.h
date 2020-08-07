#ifndef OBJECT_H
#define OBJECT_H

#include "common_includes.h"


class EngineObject
{
private:
	GLuint vertex_array_object;
	GLuint vertex_buffer_object;
	GLuint vshader;
	GLuint fshader;

public:
	EngineObject();
	EngineObject(GLfloat * )
};


#endif