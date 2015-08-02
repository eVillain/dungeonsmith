#version 400

out vec4 color;

in vec2 vTexCoord;

uniform sampler2D textureMap;
uniform float nearDepth;    // Camera z near
uniform float farDepth;     // Camera z far

float LinearizeDepth(vec2 uv) {
    float z = texture(textureMap, uv).r;
    return (2.0 * nearDepth) / (farDepth + nearDepth - z * (farDepth - nearDepth));
}

void main() {
    float depth = LinearizeDepth(vTexCoord);
	color = vec4(depth,depth,depth,1);
}

