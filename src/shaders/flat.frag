#version 330 core

out vec4 FragColor;

flat in vec3 color;
in vec2 texCoord;

in vec3 normCoord;
in vec3 currentPos;

uniform float ambient;
uniform vec3 lightPos;  

void main() {
    float light = 1.0 - abs(normalize(lightPos).x);
    //float light = 1.0;
    FragColor = vec4(ambient*light*color, 1.0f);
}
