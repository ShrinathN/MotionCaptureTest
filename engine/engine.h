#ifndef ENGINE_H
#define ENGINE_H

#include "common_includes.h"

typedef void (*drawing_function)(void *);

class EngineEngine
{
private:
	GLFWwindow * window;
	GLuint window_width;
	GLuint window_height;
	GLuint anit_aliasing;
	char * window_title;

	//color variables
	GLuint clear_r, clear_g, clear_b;


	//preprocessing function pointers
	GLuint preprocess_function_pointers = 0;
	typedef struct _preprocess_functions_ {
		void (*preprocess_functions)(void *);
		void * argument;
	}preprocess_functions[100];

	//drawing function pointers
	void * draw_functions[100];
	GLuint draw_function_pointers = 0;


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

	void ProcessingLoop();

	GLFWwindow * GetWindow();
};



#endif