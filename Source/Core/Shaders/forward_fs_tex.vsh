#version 400

layout(location = 0) in vec4 vCoord;

out vec2 vTexCoord;

void main() {
	gl_Position = vCoord;
	vTexCoord = vCoord.xy * 0.5 + 0.5;
}
