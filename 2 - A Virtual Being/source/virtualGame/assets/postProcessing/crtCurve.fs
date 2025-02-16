#version 460 core

out vec4 fragColor;

uniform sampler2D u_base;

in vec2 f_uv;

void main() {
	vec2 center = vec2(0.5f, 0.5f);
	float uvDist = (length(center - f_uv)); // / sqrt(2)
	vec4 screenBleed = vec4(0.5f) * pow(uvDist, 5.0f);

	vec4 uvColor = vec4(f_uv.x, f_uv.y, 0.0f, 0.0f) * 0.1;

    fragColor = texture2D(u_base, f_uv) + uvColor + screenBleed;
}