#version 330 core

layout(location = 0) out vec4 diffuseColor;
layout(location = 1) out vec4 specularColor;
layout(location = 2) out vec3 normal;
layout(location = 3) out float depth;

// Interpolated values from the vertex shaders
in Fragment {
    vec4 color;
    vec2 texCoord;
    in float depth;
} fragment;

// Values that stay constant for the whole mesh
uniform sampler2D textureMap;
uniform float texScale = 1.0;

void main(){
	vec4 finalColor = texture(textureMap, fragment.texCoord*texScale);
    finalColor *= fragment.color;
    
    if ( finalColor.a < 0.0001 ) discard;
    
    diffuseColor = finalColor;
    specularColor = finalColor;
    normal = vec3(0.0,1.0,0.0);
    depth = fragment.depth;
}
