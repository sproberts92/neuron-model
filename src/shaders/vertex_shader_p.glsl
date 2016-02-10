#version 450

uniform mat4 VP;
uniform mat4 MM;

in vec3 bv;
in vec3 pp;
in vec3 pc;

in vec3 colour;
in vec2 texcoord;

out vec2 Texcoord;
out vec3 Colour;

uniform vec3 CameraRight_worldspace;
uniform vec3 CameraUp_worldspace;

void main()
{
	vec3 pp_w = 0.1 * (MM * vec4(pp, 1.0)).xyz;
	float pp_s = 0.05;
	vec3 vp_worldspace = vec3((CameraRight_worldspace * bv).x * pp_s + pp_w.x, (CameraUp_worldspace * bv).y * pp_s + pp_w.y, bv.z * pp_s + pp_w.z);
	
	vec3 vp_worldspace_2 = pp_w
	+ CameraRight_worldspace * bv.x * pp_s
	+ CameraUp_worldspace    * bv.y * pp_s;

	Colour = colour;
	Texcoord = texcoord;

	gl_Position = VP * vec4(vp_worldspace_2, 1);
}
