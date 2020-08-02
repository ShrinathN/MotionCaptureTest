#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 faceColor;
out vec3 faceColorOut;
uniform mat4 transformation_matrix;
// uniform mat4 translation_matrix;
void main()
{
	gl_Position =  transformation_matrix * vec4(aPos, 2.0);
	faceColorOut = faceColor;
}