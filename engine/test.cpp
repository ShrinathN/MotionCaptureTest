#include <engine.h>
#include <object.h>
#include <shader.h>

GLfloat data[] ={
	0.5, 0.5, 0.0,
	0.5, 0.0, 0.0,
	0.0, 0.0, 0.0
};

// GLFWwindow * window;

// int main()
// {
// 	glfwInit();
// 	window = glfwCreateWindow(1920, 1080, "h", NULL, NULL);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	glfwMakeContextCurrent(window);
// 	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

// 	GLuint vao, vbo;
// 	glGenVertexArrays(1, &vao);
// 	glGenBuffers(1, &vbo);
// 	glBindVertexArray(vao);
// 	glBindBuffer(GL_ARRAY_BUFFER, vbo);
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * (sizeof(GLfloat)), (void *)0);
// 	glEnableVertexAttribArray(0);

// 	while (1)
// 	{
// 		glDrawArrays(GL_TRIANGLES, 0, 3);
// 		glfwSwapBuffers(window);
// 		glfwPollEvents();
// 	}
// }

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
	obj0.GetVShaderObj().CompileShader();
	obj0.GetFShaderObj().CompileShader();

	EngineProgram program = EngineProgram();
	program.AddShader(obj0.GetVShaderObj());
	program.AddShader(obj0.GetFShaderObj());
	program.LinkProgram();
	program.UseProgram();

	engine.SetClearColor(1.0f, 0.0f, 0.0f);
	while (!glfwWindowShouldClose(engine.GetWindow()))
	{
		program.UseProgram();
		engine.ClearColor();
		obj0.DrawObject(3);

		glfwSwapBuffers(engine.GetWindow());
		glfwPollEvents();
	}

	return 0;
}