#version 400

layout(location = 0) in vec4 vCoord;

void main() {
	gl_Position = vCoord;
}
