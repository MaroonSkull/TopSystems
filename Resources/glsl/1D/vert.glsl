#version 330 core

in vec2 UV;

out vec3 pointColor;

uniform sampler2D renderedTexture;
uniform float time;

void main(){
    pointColor = vec3(1, 2, 255);
}