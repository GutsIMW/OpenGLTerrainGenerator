#version 430
precision highp float;

layout(location=11) uniform samplerCube envMap;
in vec3 Po;
out vec4 frag_out;

void main()
{
	frag_out = texture(envMap, Po);
}
