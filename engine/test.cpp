#include "engine.h"
#include "object.h"
#include "shader.h"
#include "shader_program.h"

GLfloat data[] ={
	0.5, 0.5, 0.0,
	0.5, 0.0, 0.0,
	0.0, 0.0, 0.0
};

int main()
{
	EngineEngine engine = EngineEngine(1920, 1080, "t");
	engine.CreateWindow();
	engine.CreateContextCurrent();

	EngineObject obj0 = EngineObject();
	obj0.SetDataArray(data, sizeof(data));
	obj0.CreateObject();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);

	obj0.CreateVShader("shader.vs");
	obj0.CreateFShader("shader.fs");

	EngineProgram program = EngineProgram();
	program.AddShader(obj0.GetVShaderGLuint());
	program.AddShader(obj0.GetFShaderGLuint());
	program.LinkProgram();
	program.UseProgram();

	engine.SetClearColor(1.0f, 0.0f, 0.0f);
	while (!glfwWindowShouldClose(engine.GetWindow()))
	{
		// program.UseProgram();
		engine.ClearColor();
		obj0.DrawObject(3);

		glfwSwapBuffers(engine.GetWindow());
		glfwPollEvents();
	}

	return 0;
}