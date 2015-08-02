#version 400

out vec4 color;

in vec2 vTexCoord;

uniform sampler2D textureMap;
uniform vec4 uColor;

void main() {
	color = texture(textureMap, vTexCoord)*uColor;
}
