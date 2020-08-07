#ifndef ENGINE_H
#define ENGINE_H

#include "common_includes.h"

class EngineEngine
{
private:
	GLFWwindow * window;
	GLuint window_width;
	GLuint window_height;
	GLuint anit_aliasing;
	char * window_title;

	GLuint clear_r;
	GLuint clear_g;
	GLuint clear_b;
public:
	EngineEngine();
	EngineEngine(GLuint width, GLuint height, const char * title);
	void ClearColor();

	//set functions
	void SetWidth(int w);
	void SetHeight(int h);
	void SetTitle(const char * title);
	void SetAA(GLuint aa);
	void CreateWindow();
	void SetClearColor(GLuint r, GLuint g, GLuint b);

	void CreateContextCurrent();

	GLFWwindow * GetWindow();
};



#endif