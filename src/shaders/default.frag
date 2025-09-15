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

    //FragColor = vec4(color, 1.0f);
    //FragColor = (mix(texture(tex0, texCoord), texture(tex1, texCoord), mixer) + vec4(color, 1.0f))*diffuse;
    FragColor = (mix(texture(tex0, texCoord), texture(tex1, texCoord), mixer) + vec4(color, 1.0f));
}
