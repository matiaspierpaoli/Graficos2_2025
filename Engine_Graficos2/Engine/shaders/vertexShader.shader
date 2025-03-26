#version 330 core
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
out vec4 ourColor;
uniform mat4 mvp;
void main()
{
	gl_Position = mvp * position;
	ourColor = color;
};