#version 330 core

layout (location = 0) in vec3 oPos;

void main()
{
	gl_Position = vec4(oPos.x, oPos.y, oPos.z, 1.0f);
}