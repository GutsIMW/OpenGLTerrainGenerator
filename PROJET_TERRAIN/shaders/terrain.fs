#version 430
precision highp float;

// UNIFORM
layout(location=12) uniform vec3 light_pos; // Light position in view space
layout(location=16) uniform vec4 colorMap[255];

// INPUT
in float z_noise; // Z noise value
in vec3 pos_inview; // Position in view space
in vec3 v_norm; // normal in view space

// OUTPUT
out vec4 oFragmentColor;

float diff_intensity() // return the intensity of the diffuse lightning
{
	vec3 N = v_norm;
	N = normalize(N);
	vec3 L = normalize(light_pos - pos_inview);
	float diff = max(0.1, dot(L, N)); // 0.1 because I want the terrain to always be at least a little lighted
	return diff;
}

// MAIN PROGRAM
void main()
{
	// COLOR MAP
	int index = int(mod(max(z_noise * 255, 0), 255));
	vec4 v_col = vec4(z_noise) * vec4(colorMap[index]);

	// LAMBERT COLOR
	float diff_intens = diff_intensity();

	// FINAL COLOR
	vec4 col = min(v_col * diff_intens, vec4(1));
	oFragmentColor = col;
}