#version 430

layout(location=1) uniform mat4 projectionMatrix;
layout(location=2) uniform mat4 viewMatrix;
layout(location=3) uniform mat4 transformMatrix;

layout(location=6) uniform float water_level;
layout(location=7) uniform float tiling;
layout(location=8) uniform vec4 cameraPos; // Camera position in world space
layout(location=9) uniform vec3 light_pos; // Light position in world space


// INPUT
layout(location = 1) in vec3 position_in;
layout(location = 2) in vec2 position_tex;

// OUTPUT
out vec2 uv;
out vec4 clipSpace;
out vec3 toCamDirection;
out vec3 fromLightDirection;
out vec3 viewPos;

// MAIN PROGRAM
void main()
{
    uv = position_tex * tiling; // compute texture coordinates
    
    vec4 Po4 = vec4(position_in.xy, water_level, 1.);

	Po4 = transformMatrix * Po4; // World position
    toCamDirection = cameraPos.xyz - Po4.xyz; // Vector from vertex to camera (world space)
    fromLightDirection = Po4.xyz - light_pos; // Vector from light to vertex (world space)

    vec4 Po4View = viewMatrix * Po4; // View position
    viewPos = Po4View.xyz;

	Po4 = projectionMatrix * Po4View; // Projection position
    clipSpace = Po4;

    gl_Position = Po4;
}