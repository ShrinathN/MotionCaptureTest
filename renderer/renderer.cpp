#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <unistd.h>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "network.hpp"

#define ALPHA 0.6
#define UPDATE_OLD_APP_DATA memcpy(&old_app_data, &app_data, sizeof(struct _app_data_))

extern struct _app_data_ app_data;
struct _app_data_ old_app_data = {0,};
GLFWwindow *window;

GLuint vao;
GLuint vbo;

GLuint vshader;
GLuint fshader;
GLuint program;

GLint scale_matrix_location;
GLint x_rotation_matrix_location;
GLint y_rotation_matrix_location;
GLint z_rotation_matrix_location;
GLint transform_matrix_location;

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

	scale_matrix_location = glGetUniformLocation(program, "scale_matrix");
	x_rotation_matrix_location = glGetUniformLocation(program, "x_rotation_matrix");
	y_rotation_matrix_location = glGetUniformLocation(program, "y_rotation_matrix");
	z_rotation_matrix_location = glGetUniformLocation(program, "z_rotation_matrix");
	transform_matrix_location = glGetUniformLocation(program, "transform_matrix");
}

void processing_loop()
{
	glm::mat4 scale_matrix = glm::mat4(1.0f);
	glm::mat4 x_rotation_matrix = glm::mat4(1.0);
	glm::mat4 y_rotation_matrix = glm::mat4(1.0);
	glm::mat4 z_rotation_matrix = glm::mat4(1.0);
	glm::mat4 transform_matrix = glm::mat4(1.0f);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(program);
	int bytes_read;
	float scale_value;

	float filtered_x;
	float filtered_y;
	
	while (!glfwWindowShouldClose(window))
	{
		bytes_read = NETWORK_read_data();
		UPDATE_OLD_APP_DATA;
		// printf("%d ", bytes_read);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(program);

		//gyroscope readings
		//x axis
		if (abs(app_data.gyro_x) < 0.35f)
			x_rotation_matrix = glm::rotate(x_rotation_matrix, glm::radians(0.0f), glm::vec3(0.001f, 0.0f, 0.0f));
		else
			x_rotation_matrix = glm::rotate(x_rotation_matrix, glm::radians(app_data.gyro_x), glm::vec3(-1.0f, 0.0f, 0.0f));
		//y axis
		if (abs(app_data.gyro_y) < 0.35f)
			y_rotation_matrix = glm::rotate(y_rotation_matrix, glm::radians(0.0f), glm::vec3(0.0f, 0.001f, 0.0f));
		else
			y_rotation_matrix = glm::rotate(y_rotation_matrix, glm::radians(app_data.gyro_y), glm::vec3(0.0f, -1.0f, 0.0f));
		//z axis
		if (abs(app_data.gyro_z) < 0.35f)
			z_rotation_matrix = glm::rotate(z_rotation_matrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 0.001f));
		else
			z_rotation_matrix = glm::rotate(z_rotation_matrix, glm::radians(app_data.gyro_z), glm::vec3(0.0f, 0.0f, 1.0f));

		//accelerometer reading. We'll ignore x,y for the time
		// scale_value = (0.01f) * (((abs(app_data.accel_z) > 3.0f) ? (app_data.accel_z) : (0)) * -1.0f);
		// scale_matrix = glm::scale(scale_matrix, glm::vec3(1.0f + scale_value, 1.0f + scale_value, 1.0f + scale_value));

		// //LPF for translation
		// filtered_x = (app_data.accel_x * (1 - ALPHA)) + (old_app_data.accel_x * ALPHA);
		// filtered_y = (app_data.accel_y * (1 - ALPHA)) + (old_app_data.accel_y * ALPHA);

		// transform_matrix = glm::translate(transform_matrix, glm::vec3(0.0f + (app_data.accel_x * ((abs(filtered_x) < 0.5f) ? 0.0f : 0.01f)), 0.0f  + (app_data.accel_y * ((abs(filtered_y) < 0.5f) ? 0.0f : 0.01f)), 0.0f));

		//reset
		if( glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS )
		{
			scale_matrix = glm::mat4(1.0f);
			x_rotation_matrix = glm::mat4(1.0f);
			y_rotation_matrix = glm::mat4(1.0f);
			z_rotation_matrix = glm::mat4(1.0f);
			transform_matrix = glm::mat4(1.0f);
		}

		glUniformMatrix4fv(scale_matrix_location, 1, GL_FALSE, glm::value_ptr(scale_matrix));
		glUniformMatrix4fv(x_rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(x_rotation_matrix));
		glUniformMatrix4fv(y_rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(y_rotation_matrix));
		glUniformMatrix4fv(z_rotation_matrix_location, 1, GL_FALSE, glm::value_ptr(z_rotation_matrix));
		glUniformMatrix4fv(transform_matrix_location, 1, GL_FALSE, glm::value_ptr(transform_matrix));
		glBindVertexArray(vao);
		glDrawArrays(GL_TRIANGLES, 0, sizeof(cube_data) / (sizeof(GLfloat) * 6));

		glfwSwapBuffers(window);
		glfwPollEvents();

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
	glfwWindowHint(GLFW_SAMPLES, 16);
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