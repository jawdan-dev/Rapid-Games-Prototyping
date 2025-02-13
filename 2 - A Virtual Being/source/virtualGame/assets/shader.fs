#version 460 core

out vec4 fragColor;

in vec3 colorOffset;


void main() {
    fragColor = vec4(1.0f + colorOffset.r, 0.5f + colorOffset.g, 0.2f + colorOffset.b, 1.0f);
}