#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

in vec3 normCoord;
in vec3 currentPos;

uniform sampler2D tex0;
uniform sampler2D tex1;

uniform float mixer;

void main() {
    vec3 norm = normalize(vec3(texCoord, 1.0f));
    vec3 lightDirection = normalize(vec3(16.0f, 16.0f, 16.0f));
    float diffuse = max(dot(currentPos, lightDirection), 0.0f);

    if(mixer < 0.5) {
        FragColor = vec4(color, 1.0f);
    }
    else {
        vec4 texture1 = texture(tex0, texCoord);
        vec4 texture2 = texture(tex1, texCoord);
        
        FragColor = mix(texture1, texture2, 0.5);
    }
}
