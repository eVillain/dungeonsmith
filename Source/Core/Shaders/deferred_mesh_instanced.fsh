#version 330 core
precision highp float;

// Fragment shader for instancing of colored cubes
// Will output color(diffuse+specular), normal and depth values

layout(location = 0) out vec4 diffuseColor;
layout(location = 1) out vec4 specularColor;
layout(location = 2) out vec3 normal;
layout(location = 3) out float depth;

in Fragment {
    smooth vec4 diffuse;
    smooth vec4 specular;
    vec3 normal;
    smooth float depth;
     vec3 pos;
} fragment;


//float hash( float n )
//{
//    return fract(sin(n)*43758.5453);
//}
//
//float noise( vec3 x )
//{
//    // The noise function returns a value in the range -1.0f -> 1.0f
//    
//    vec3 p = floor(x);
//    vec3 f = fract(x);
//    
//    f       = f*f*(3.0-2.0*f);
//    float n = p.x + p.y*57.0 + 113.0*p.z;
//    
//    return mix(mix(mix( hash(n+0.0), hash(n+1.0),f.x),
//                   mix( hash(n+57.0), hash(n+58.0),f.x),f.y),
//               mix(mix( hash(n+113.0), hash(n+114.0),f.x),
//                   mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
//}

void main(void) {
    
//    float fragNoise = noise(fragment.pos.xyz*128.0);
    
    diffuseColor = fragment.diffuse;
    specularColor = fragment.specular;
    normal = (fragment.normal+1.0)*0.5;
    depth = fragment.depth;
}

