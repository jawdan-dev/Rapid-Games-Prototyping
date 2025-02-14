#version 460 core

out vec4 fragColor;

uniform sampler2D u_base;

in vec2 f_uv;

void main() {
    fragColor = texture2D(u_base, f_uv);// + vec4(f_uv.x, f_uv.y, 0.0f, 0.0f);
}