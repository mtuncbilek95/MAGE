#version 460

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBitangent
layout(location = 4) in vec4 inColor;
layout(location = 5) in vec2 inUV;

layout(location = 0) out vec4 outColor;

void main()
{
	gl_Position = vec4(inPos.x, -inPos.y, inPos.z, 1.0);
	outColor = inColor;
}