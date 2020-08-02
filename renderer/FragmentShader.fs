#version 330 core

out vec4 colOut;
in vec3 faceColorOut;

void main()
{
	colOut = vec4(faceColorOut, 1.0);
}