#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
uniform vec2 offset;
out vec3 outColor;

void main()
{
	//gl_Position = vec4(aPos, 1.0);
	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0) +vec4(offset.x, offset.y, 0, 0);
	outColor = aColor;
}