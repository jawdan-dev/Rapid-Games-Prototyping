#version 460 core

out vec4 fragColor;

uniform sampler2D u_base;
uniform sampler2D u_dots;
uniform sampler2D u_dotMap;
uniform float u_dotsAspect;
uniform float u_dotsScale;

in vec2 f_uv;

void main() {
	vec2 dotUV = vec2(f_uv.x, f_uv.y * u_dotsAspect) * u_dotsScale;
	vec2 dotColorUV = (dotUV - fract(dotUV)) + texture2D(u_dotMap, dotUV).xy;
	vec2 sampleUV = vec2(dotColorUV.x, dotColorUV.y / u_dotsAspect) / u_dotsScale;
    fragColor = texture2D(u_base, sampleUV) * texture2D(u_dots, dotUV);
}