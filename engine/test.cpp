#include <engine.h>
#include <object.h>
#include <shader.h>

GLfloat data[] ={
	0.5, 0.5, 0.0,
	0.5, 0.0, 0.0,
	0.0, 0.0, 0.0
};

int main()
{
	EngineEngine engine = EngineEngine();
	engine.SetHeight(1080);
	engine.SetWidth(1920);
	engine.SetTitle("Test");
	engine.CreateWindow();
	engine.CreateContextCurrent();


	EngineObject obj0 = EngineObject();
	obj0.CreateObject();
	obj0.SetDataArray(data, sizeof(data));
	obj0.CreateObject();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);


	obj0.CreateVShader("shader.vs");
	obj0.GetVShaderObj().CompileShader();


	obj0.CreateFShader("shader.fs");
	obj0.GetFShaderObj().CompileShader();

	EngineProgram program = EngineProgram();
	program.AddShader(obj0.GetVShaderObj());
	program.AddShader(obj0.GetFShaderObj());
	program.LinkProgram();
	program.UseProgram();

	while( !glfwWindowShouldClose(engine.GetWindow()) )
	{
		obj0.DrawObject(3);
		glfwSwapBuffers(engine.GetWindow());
		glfwPollEvents();
	}

	return 0;
}