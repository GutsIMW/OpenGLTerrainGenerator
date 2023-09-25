#version 430

// UNIFORM
layout(location = 0) uniform uint uTerrainSize; // Total number of vertices
layout(location = 1) uniform mat4 projectionMatrix;
layout(location = 2) uniform mat4 viewMatrix;
layout(location = 3) uniform mat4 transformMatrix;
layout(location = 4) uniform vec4 water_plane;

layout(location = 6) uniform float noise_freq_1; // Noise frequency octave 1
layout(location = 7) uniform float noise_freq_2; // Noise frequency octave 2
layout(location = 8) uniform float noise_freq_3; // Noise frequency octave 3
layout(location = 13) uniform float noise_freq_4; // Noise frequency octave 4
layout(location = 9) uniform float noise_freq; // Global noise frequency

layout(location = 10) uniform float exponent; // Redistribution factor
layout(location = 11) uniform float linear_reshape_ratio; // Square bump linear reshaping ratio (allows to control if the terrain looks more like a single island or more like an archipelago)
layout(location = 5) uniform uint linear_reshaping_mode;
layout(location = 15) uniform sampler2D heightmap;
layout(location = 14) uniform mat3 normalViewMatrix;

// INPUT
layout(location = 1) in vec3 position_in;
layout(location = 2) in vec2 position_tex;

// OUTPUT
out float z_noise;
out vec3 pos_inview;
out vec3 v_norm; // Normal in view space

// Some useful functions
vec3 mod289(vec3 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec2 mod289(vec2 x) { return x - floor(x * (1.0 / 289.0)) * 289.0; }
vec3 permute(vec3 x) { return mod289(((x*34.0)+1.0)*x); }

//
// Description : GLSL 2D simplex noise function
//      Author : Ian McEwan, Ashima Arts
//  Maintainer : ijm
//     Lastmod : 20110822 (ijm)
//     License :
//  Copyright (C) 2011 Ashima Arts. All rights reserved.
//  Distributed under the MIT License. See LICENSE file.
//  https://github.com/ashima/webgl-noise
//
float snoise(vec2 v) {

    // Precompute values for skewed triangular grid
    const vec4 C = vec4(0.211324865405187,
                        // (3.0-sqrt(3.0))/6.0
                        0.366025403784439,
                        // 0.5*(sqrt(3.0)-1.0)
                        -0.577350269189626,
                        // -1.0 + 2.0 * C.x
                        0.024390243902439);
                        // 1.0 / 41.0

    // First corner (x0)
    vec2 i  = floor(v + dot(v, C.yy));
    vec2 x0 = v - i + dot(i, C.xx);

    // Other two corners (x1, x2)
    vec2 i1 = vec2(0.0);
    i1 = (x0.x > x0.y)? vec2(1.0, 0.0):vec2(0.0, 1.0);
    vec2 x1 = x0.xy + C.xx - i1;
    vec2 x2 = x0.xy + C.zz;

    // Do some permutations to avoid
    // truncation effects in permutation
    i = mod289(i);
    vec3 p = permute(
            permute( i.y + vec3(0.0, i1.y, 1.0))
                + i.x + vec3(0.0, i1.x, 1.0 ));

    vec3 m = max(0.5 - vec3(
                        dot(x0,x0),
                        dot(x1,x1),
                        dot(x2,x2)
                        ), 0.0);

    m = m*m ;
    m = m*m ;

    // Gradients:
    //  41 pts uniformly over a line, mapped onto a diamond
    //  The ring size 17*17 = 289 is close to a multiple
    //      of 41 (41*7 = 287)

    vec3 x = 2.0 * fract(p * C.www) - 1.0;
    vec3 h = abs(x) - 0.5;
    vec3 ox = floor(x + 0.5);
    vec3 a0 = x - ox;

    // Normalise gradients implicitly by scaling m
    // Approximation of: m *= inversesqrt(a0*a0 + h*h);
    m *= 1.79284291400159 - 0.85373472095314 * (a0*a0+h*h);

    // Compute final noise value at P
    vec3 g = vec3(0.0);
    g.x  = a0.x  * x0.x  + h.x  * x0.y;
    g.yz = a0.yz * vec2(x1.x,x2.x) + h.yz * vec2(x1.y,x2.y);
    return 130.0 * dot(m, g);
}


// Compute a noisy elevation to apply to the vertex according to its 2D position
float noised_elevation(vec2 pos_xy)
{
    pos_xy = noise_freq * pos_xy; // Global noise frequency
    float elevation = 1. * snoise(noise_freq_1 * pos_xy); // First octave
    elevation += (1./2.) * snoise(noise_freq_2 * pos_xy + vec2(5.3, 9.1)); // Second octave
    elevation += (1./4.) * snoise(noise_freq_3 * pos_xy + vec2(17.8, 23.5)); // Third octave
    elevation += (1./8.) * snoise(noise_freq_4 * pos_xy + vec2(27.9, 32.8)); // Fourth octave

    elevation = elevation / (1. + 1./2. + 1./4. + 1./8.); // keep the elevation in [-1, 1]
    elevation = (elevation*.5+.5); // elevation is now in [0, 1]

    // LINEAR RESHAPING
    float nx = pos_xy.x / (noise_freq);
    float ny = pos_xy.y / (noise_freq);
    float d;
    switch(linear_reshaping_mode) // Decide which linear reshaping function to apply
    {
        case 0: // Square Bump
            d = 1 - (1- pow(nx, 2)) * (1- pow(ny, 2));
            break;
        case 1: // Euclidian Squared
            d = min(1, (pow(nx, 2) + pow(ny, 2)) / sqrt(2)); 
            break;
        case 2: // "Star" distance
            d = abs(pow(nx, 2) + pow(ny, 2)) / sqrt(2) * 2.7 / (3. - sin(5. * atan(ny, nx)));
            break;
        case 3: // Diagonal
            d = max(abs(nx), abs(ny));
            break;
        case 4: // Manhattan
            d = (abs(nx) + abs(ny)) / 2.;
            break;
        case 5: // Hyperboloid
            d = sqrt(nx*nx + ny*ny) / sqrt(2);
            break;
        case 6: // Use a texture ("skull.jfif")
            d = 1 - texture(heightmap, position_tex).r;
            break;
        default: // By default, Square Bump
            d = 1 - (1- pow(nx, 2)) * (1- pow(ny, 2));
    }
    float linear_reshape = 1-d;
    elevation = mix(elevation, linear_reshape, linear_reshape_ratio); // the final elevation is a mix between the previous elevation and a square bump

    // Redistribution
    elevation = pow(elevation, exponent);

    return elevation;
}

vec3 computeNormal(vec3 A, vec3 B, vec3 C) {
    return normalize(cross(B - A, C - A));
}

vec3 computeNormal(vec3 pos)
{
    float step = 1.f / float(uTerrainSize - 1);

	// Computes the normal based on the 6 neighbor's triangle normals
	vec2 l2d = vec2(pos.x - step, pos.y);
	vec2 t2d = vec2(pos.x, pos.y - step);
	vec2 r2d = vec2(pos.x + step, pos.y);
	vec2 b2d = vec2(pos.x, pos.y + step);
	vec2 tl2d = vec2(pos.x - step, pos.y - step);
	vec2 br2d = vec2(pos.x + step, pos.y + step);

	vec3 l = vec3(l2d.x, l2d.y, pos.z + noised_elevation(l2d));
	vec3 t = vec3(t2d.x, t2d.y, pos.z + noised_elevation(t2d));
	vec3 r = vec3(r2d.x, r2d.y, pos.z + noised_elevation(r2d));
	vec3 b = vec3(b2d.x, b2d.y, pos.z + noised_elevation(b2d));
	vec3 tl = vec3(tl2d.x, tl2d.y, pos.z + noised_elevation(tl2d));
	vec3 br = vec3(br2d.x, br2d.y, pos.z + noised_elevation(br2d));

	vec3 n1 = computeNormal(pos, l, tl);
	vec3 n2 = computeNormal(pos, tl, t);
	vec3 n3 = computeNormal(pos, t, r);
	vec3 n4 = computeNormal(pos, r, br);
	vec3 n5 = computeNormal(pos, br, b);
	vec3 n6 = computeNormal(pos, b, l);

	vec3 normal = normalize(n1 + n2 + n3 + n4 + n5 + n6);

	return normal;
}

// MAIN PROGRAM
void main()
{
	vec2 pos_xy = position_in.xy;
    z_noise = noised_elevation(pos_xy); // Compute a random elevation

	float po_z = position_in.z + z_noise; // Apply elevation
	vec4 Po4 = vec4(position_in.x, position_in.y, po_z, 1);
    vec4 world_Po4 = transformMatrix * Po4; // World position
    
    // get the normal
	vec3 object_normal = computeNormal(Po4.xyz); // Normal in the object space
    v_norm = normalViewMatrix * object_normal; // Normal in view space

    // Clip geometry for reflection and refraction
    gl_ClipDistance[0] = dot(world_Po4, water_plane);

	vec4 view_Po4 = viewMatrix * world_Po4; // View position
    pos_inview = view_Po4.xyz;

	vec4 projected_Po4 = projectionMatrix * view_Po4; // Projection position

	gl_Position = projected_Po4;
}