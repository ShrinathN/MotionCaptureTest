#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 faceColor;
out vec3 faceColorOut;
uniform mat4 transformation_matrix;
uniform mat4 x_rotation_matrix;
uniform mat4 y_rotation_matrix;
uniform mat4 z_rotation_matrix;
void main()
{
	gl_Position = x_rotation_matrix * y_rotation_matrix * z_rotation_matrix * vec4(aPos, 2.0);
	faceColorOut = faceColor;
}