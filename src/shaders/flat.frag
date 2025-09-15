#version 330 core

out vec4 FragColor;

flat in vec3 color;
in vec2 texCoord;

in vec3 normCoord;
in vec3 currentPos;


uniform float mixer;

void main() {

    FragColor = vec4(color, 1.0f);

}