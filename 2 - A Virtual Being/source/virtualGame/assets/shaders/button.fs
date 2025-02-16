#version 460 core

out vec4 fragColor;

in vec3 f_col;

void main() {
    fragColor = vec4(f_col.r, f_col.g, f_col.b, 1.0f);
}