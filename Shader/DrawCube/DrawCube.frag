#version 450

layout(location = 0) in vec3 FragPos;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 TexCoords;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(TexCoords, 1.0, 1.0);
} 