#version 450

uniform mat4 MVP;

in vec3 position;
in vec3 colour;
in vec2 texcoord;

out vec3 Colour;
out vec2 Texcoord;

void main()
{
	Colour = colour;
	Texcoord = texcoord;

	gl_Position = MVP * vec4(position, 1);
}
