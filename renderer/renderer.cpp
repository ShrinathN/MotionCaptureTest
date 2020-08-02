#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "network.hpp"

extern struct _app_data_ app_data;

GLFWwindow * window;

GLuint vao;
GLuint vbo;

GLuint vshader;
GLuint fshader;
GLuint program;

GLint transformation_matrix_location;
GLint translation_matrix_location;

const GLfloat cube_data[] = {
	//down
	-0.25, -0.25, -0.25, 1.0, 0.0, 0.0, //red
	+0.25, -0.25, -0.25, 1.0, 0.0, 0.0,
	-0.25, -0.25, +0.25, 1.0, 0.0, 0.0,

	+0.25, -0.25, -0.25, 1.0, 0.0, 0.0,
	-0.25, -0.25, +0.25, 1.0, 0.0, 0.0,
	+0.25, -0.25, +0.25, 1.0, 0.0, 0.0,

	//left
	-0.25, -0.25, +0.25, 0.0, 1.0, 0.0, //green
	-0.25, -0.25, -0.25, 0.0, 1.0, 0.0,
	-0.25, +0.25, +0.25, 0.0, 1.0, 0.0,

	-0.25, +0.25, +0.25, 0.0, 1.0, 0.0,
	-0.25, -0.25, -0.25, 0.0, 1.0, 0.0,
	-0.25, +0.25, -0.25, 0.0, 1.0, 0.0,

	//top
	-0.25, +0.25, +0.25, 0.0, 0.0, 1.0,
	-0.25, +0.25, -0.25, 0.0, 0.0, 1.0,
	+0.25, +0.25, -0.25, 0.0, 0.0, 1.0,

	+0.25, +0.25, -0.25, 0.0, 0.0, 1.0,
	+0.25, +0.25, +0.25, 0.0, 0.0, 1.0,
	-0.25, +0.25, +0.25, 0.0, 0.0, 1.0,

	//right
	+0.25, -0.25, -0.25, 1.0, 1.0, 0.0,
	+0.25, -0.25, +0.25, 1.0, 1.0, 0.0,
	+0.25, +0.25, +0.25, 1.0, 1.0, 0.0,

	+0.25, -0.25, -0.25, 1.0, 1.0, 0.0,
	+0.25, +0.25, -0.25, 1.0, 1.0, 0.0,
	+0.25, +0.25, +0.25, 1.0, 1.0, 0.0,

	//back
	+0.25, -0.25, -0.25, 1.0, 1.0, 1.0,
	+0.25, +0.25, -0.25, 1.0, 1.0, 1.0,
	-0.25, +0.25, -0.25, 1.0, 1.0, 1.0,

	-0.25, +0.25, -0.25, 1.0, 1.0, 1.0,
	+0.25, -0.25, -0.25, 1.0, 1.0, 1.0,
	-0.25, -0.25, -0.25, 1.0, 1.0, 1.0};

char *vshader_str;
char *fshader_str; 

char *loadShader(const char *filename)
{
	FILE *in = fopen(filename, "r");
	fseek(in, 0, SEEK_END);
	unsigned long file_length = ftell(in);
	fseek(in, 0, SEEK_SET);
	char *to_return_ptr = (char *)malloc(file_length + 1);
	bzero(to_return_ptr, file_length + 1);
	fread(to_return_ptr, file_length, 1, in);
	fclose(in);
	return to_return_ptr;
}


void checkShader(GLuint *shader)
{
	int success;
	char buffer[500];
	glGetShaderiv(*shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(*shader, sizeof(buffer), NULL, buffer);
		printf("%s\n", buffer);
	}
}

void shader_init()
{
	vshader = glCreateShader(GL_VERTEX_SHADER);
	fshader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vshader, 1, &vshader_str, NULL);
	glShaderSource(fshader, 1, &fshader_str, NULL);
	glCompileShader(vshader);
	glCompileShader(fshader);
	checkShader(&vshader);
	checkShader(&fshader);

	program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	glUseProgram(program);

	transformation_matrix_location = glGetUniformLocation(program, "transformation_matrix");
	// translation_matrix_location = glGetUniformLocation(program, "translation_matrix");
}

void processing_loop()
{
	glm::mat4 transformation_matrix = glm::mat4(1.0f);
	glm::mat4 translation_matrix = glm::mat4(1.0f);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);
	float z_translate = 1.0f;
	while (!glfwWindowShouldClose(window))
	{
		// NETWORK_read_data();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);

		transformation_matrix = glm::rotate(transformation_matrix, glm::radians(1.0f), glm::vec3(0.0,0.0,0.0));
		// translation_matrix = glm::scale(translation_matrix, glm::vec3(z_translate, z_translate, z_translate));

		glUniformMatrix4fv(transformation_matrix_location, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
		// glUniformMatrix4fv(translation_matrix_location, 1, GL_FALSE, glm::value_ptr(translation_matrix));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_data) / (sizeof(GLfloat) * 6));

		glfwSwapBuffers(window);
		glfwPollEvents();

	
		printf("%f\n", z_translate);
	}
}

void figure_init()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_data), cube_data, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);
}

void window_init()
{
	glfwInit();
	window = glfwCreateWindow(1920, 1080, "3D_Test", NULL, NULL);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

int main()
{
	NETWORK_create_socket();
	window_init();
	vshader_str = loadShader("VertexShader.vs");
	fshader_str = loadShader("FragmentShader.fs");
	figure_init();
	shader_init();

	processing_loop();
}