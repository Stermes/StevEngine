// shadertype = <glsl>
#version 400 core

layout (location = 0) in vec2 position;
layout (location = 2) in vec2 texCoords;

out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 0, 1.0f);
	FragPos = vec3(model * vec4(position, 0, 1.0f));
	TexCoords = texCoords;
}