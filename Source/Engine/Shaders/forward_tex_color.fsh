#version 400

out vec4 color;

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
	color = texture(textureMap, fragment.texCoord*texScale) * fragment.color;
}
