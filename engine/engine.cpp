#include "engine.h"

EngineEngine::EngineEngine()
{
	return;
}

EngineEngine::EngineEngine(GLuint width, GLuint height, const char * title)
{
	this->SetWidth(width);
	this->SetHeight(height);
	this->SetTitle(title);
}

void EngineEngine::CreateWindow()
{
	glfwInit();
	this->window = glfwCreateWindow(this->window_width, this->window_height, this->window_title, NULL, NULL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void EngineEngine::CreateContextCurrent()
{
	glfwMakeContextCurrent(this->window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}


void EngineEngine::SetWidth(int w)
{
	this->window_width = w;
}

void EngineEngine::SetHeight(int h)
{
	this->window_height = h;
}

void EngineEngine::SetTitle(const char * title)
{
	this->window_title = (char *)title;
}

void EngineEngine::SetAA(GLuint aa)
{
	this->anit_aliasing = aa;
}

GLFWwindow * EngineEngine::GetWindow()
{
	return this->window;
}