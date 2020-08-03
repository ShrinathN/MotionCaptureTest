#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <unistd.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "network.hpp"

extern struct _app_data_ app_data;
struct _app_data_ old_app_data = {0,};
GLFWwindow *window;

GLuint vao;
GLuint vbo;

GLuint vshader;
GLuint fshader;
GLuint program;

GLint transformation_matrix_location;
GLint x_rotation_matrix_location;
GLint y_rotation_matrix_location;
GLint z_rotation_matrix_location;

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
	x_rotation_matrix_location = glGetUniformLocation(program, "x_rotation_matrix");
	y_rotation_matrix_location = glGetUniformLocation(program, "y_rotation_matrix");
	z_rotation_matrix_location = glGetUniformLocation(program, "z_rotation_matrix");
}

void processing_loop()
{
	glm::mat4 transformation_matrix = glm::mat4(1.0f);
	glm::mat4 x_rotation_matrix = glm::mat4(1.0);
	glm::mat4 y_rotation_matrix = glm::mat4(1.0);
	glm::mat4 z_rotation_matrix = glm::mat4(1.0);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);
	int bytes_read;
	
	while (!glfwWindowShouldClose(window))
	{
		bytes_read = NETWORK_read_data();
		// printf("%d ", bytes_read);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);

		// transformation_matrix = glm::rotate(transformation_matrix, glm::radians(0.001f), glm::vec3(0.001f, 0.0f, 0.0f));
		if (abs(app_data.gyro_x) < 0.35f)
		{
			x_rotation_matrix = glm::rotate(x_rotation_matrix, glm::radians(0.0f), glm::vec3(0.001f, 0.0f, 0.0f));
		}
		else
		{
			x_rotation_matrix = glm::rotate(x_rotation_matrix, glm::radians(app_data.gyro_x), glm::vec3(-1.0f, 0.0f, 0.0f));
		}

		if (abs(app_data.gyro_y) < 0.35f)
		{
			y_rotation_matrix = glm::rotate(y_rotation_matrix, glm::radians(0.0f), glm::vec3(0.0f, 0.001f, 0.0f));
		}
		else
		{
			y_rotation_matrix = glm::rotate(y_rotation_matrix, glm::radians(app_data.gyro_y), glm::vec3(0.0f, -1.0f, 0.0f));
		}

		if (abs(app_data.gyro_z) < 0.35f)
		{
			z_rotation_matrix = glm::rotate(z_rotation_matrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.001f));
		}
		else
		{
			z_rotation_matrix = glm::rotate(z_rotation_matrix, glm::radians(app_data.gyro_z), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		glUniformMatrix4fv(transformation_matrix_location, 1, GL_FALSE, glm::value_ptr(transformation_matrix));
		glUniformMatrix4fv(x_rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(x_rotation_matrix));
		glUniformMatrix4fv(y_rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(y_rotation_matrix));
		glUniformMatrix4fv(z_rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(z_rotation_matrix));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_data) / (sizeof(GLfloat) * 6));

		glfwSwapBuffers(window);
		glfwPollEvents();

		// printf("%f %f %f %f %f %f\n",
		// 	   app_data.gyro_x,
		// 	   app_data.gyro_y,
		// 	   app_data.gyro_z,
		// 	   old_gyro_x,
		// 	   old_gyro_y,
		// 	   old_gyro_z);

		// usleep(16666);
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
	window_init();
	vshader_str = loadShader("VertexShader.vs");
	fshader_str = loadShader("FragmentShader.fs");
	figure_init();
	shader_init();

	NETWORK_create_socket();
	processing_loop();
}