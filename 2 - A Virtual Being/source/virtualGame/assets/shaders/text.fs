#version 460 core

out vec4 fragColor;

uniform sampler2D u_font;

in vec2 f_uv;
in vec3 f_color;

void main() {
	vec4 col = texture2D(u_font, f_uv) * vec4(f_color.rgb, 1.0f);
	if (col.a <= 0.0f) discard;
    fragColor = col;
}