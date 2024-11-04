#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent;
layout(location = 4) in vec4 inColor;
layout(location = 5) in vec2 inUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform descBuffer
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