#version 400

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec4 vCoord;
layout(location = 1) in vec2 vTex;

// Output data, will be interpolated for each fragment.
out Fragment {
    vec4 color;
    vec2 texCoord;
    float depth;
} fragment;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform vec4 uColor;

void main() {
	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  MVP * vec4(vCoord.xyz, 1);
    // Output texture coordinate, automatically interpolated
    fragment.texCoord = vTex;
    // Output color and depth
    fragment.color = uColor;
    fragment.depth = gl_Position.z;
}
