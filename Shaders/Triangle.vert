#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec4 inColor;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform mvpData
{
	mat4 model;
	mat4 view;
	mat4 proj;
} mvp;

void main()
{
	gl_Position = mvp.proj * mvp.view * mvp.model * vec4(inPos.xyz, 1.0);
	outColor = inColor;
}