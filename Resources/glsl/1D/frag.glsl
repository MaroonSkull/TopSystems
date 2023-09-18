#version 330 core

in vec3 pointColor;

layout(location = 0) out vec3 color;

void main() {
	color = vec3(pointColor.r, pointColor.g, pointColor.b);
}