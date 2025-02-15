#version 460 core

out vec4 fragColor;

uniform sampler2D u_font;

in vec2 f_uv;

void main() {
	vec4 col = texture2D(u_font, f_uv);
	if (col.a <= 0) discard;
    fragColor = col;
}