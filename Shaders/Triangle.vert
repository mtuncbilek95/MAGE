#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 outColor;

void main()
{
	gl_Position = vec4(inPos.x, -inPos.y, inPos.z, 1.0);
	outColor = vec4(inColor, 1.0);
}