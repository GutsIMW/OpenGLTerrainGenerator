#version 430

layout(location=1) uniform mat4 projectionMatrix;
layout(location=2) uniform mat4 viewMatrix;
layout(location=3) uniform mat4 transformMatrix;

layout(location=1) in vec3 position_in;

out vec3 Po;

void main()
{
	vec4 Po4 = viewMatrix * vec4(position_in,1);
	Po = position_in;
	gl_Position = (projectionMatrix * Po4).xyww;
}
