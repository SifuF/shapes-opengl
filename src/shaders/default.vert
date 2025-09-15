#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec2 aTex;
layout (location = 3) in vec3 aNor;

out vec3 color;
out vec2 texCoord;

out vec3 normCoord;
out vec3 currentPos;

uniform float scale;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    texCoord = aTex;
    normCoord = aNor;
    color = aCol;
    currentPos = vec3(model * vec4(aPos, 1.0f));

    //gl_Position = vec4(aPos.x + aPos.x*scale, aPos.y + aPos.y*scale, aPos.z + aPos.z*scale, 1.0);
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}
