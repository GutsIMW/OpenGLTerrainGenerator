#version 430
precision highp float;

// UNIFORM
layout(location=11) uniform sampler2D reflectionTex;
layout(location=12) uniform sampler2D refractionTex;
layout(location=13) uniform sampler2D distortionTex;
layout(location=14) uniform float waveStrength;
layout(location=15) uniform float ctime;
layout(location=16) uniform sampler2D normalMap;

// INPUT
in vec2 uv;
in vec4 clipSpace;
in vec3 toCamDirection;
in vec3 fromLightDirection;
in vec3 viewPos;

// OUTPUT
out vec4 oFragmentColor;

// CONST
const float specStrength = 30.0;
const float reflectivity = 0.8;

// MAIN PROGRAM
void main()
{
	// NDC
	vec2 ndc = (clipSpace.xy/clipSpace.w)/2. + 0.5;
	vec2 reflectionTexCoords = vec2(ndc.x, ndc.y);
	vec2 refractionTexCoords = vec2(ndc.x, ndc.y);

	// WAVE MOVEMENT
	float waveMovementVal = mod(ctime * 0.06, 1);

	// DISTORTION	
	vec2 distortionTexCoords_1 = vec2(uv.x + waveMovementVal, uv.y);
	vec2 distortionTexCoords_2 = vec2(-uv.x + waveMovementVal, uv.y + waveMovementVal);

	vec2 distortion_1 = (texture(distortionTex, distortionTexCoords_1).rg * 2. - 1.) * waveStrength;
	vec2 distortion_2 = (texture(distortionTex, distortionTexCoords_2).rg * 2. - 1.) * waveStrength;
	vec2 realisticDistortion = distortion_1 + distortion_2;

	// NORMAL
	vec4 normalTex = texture(normalMap, realisticDistortion);
	vec3 normal = vec3(normalTex.r * 2. - 1., normalTex.b, normalTex.g * 2. - 1.);
	normal = normalize(normal); // Normal in world space

	// TEX COORDINATES
	refractionTexCoords += realisticDistortion;
	reflectionTexCoords += realisticDistortion;

	// TEX COLORS
	vec4 reflectionCol = texture(reflectionTex, reflectionTexCoords);
	vec4 refractionCol = texture(refractionTex, refractionTexCoords);

	// FRESNEL
	vec3 viewVector = normalize(toCamDirection);
	float refractiveFactor = abs(dot(viewVector, vec3(0., 1., 0.))); // Indice de réfraction, valeur absolue pour au dessus et sous l'eau
	
	// SPECULAR REFLECT
	vec3 reflectedLight = reflect(normalize(fromLightDirection), normal);
	float spec = max(dot(reflectedLight, viewVector), 0.0);
	spec = pow(spec, specStrength);
	vec4 color_spec = vec4(1.) *  spec * reflectivity;

	// FINAL COLOR
	oFragmentColor = mix(reflectionCol, refractionCol, refractiveFactor) + color_spec;
}