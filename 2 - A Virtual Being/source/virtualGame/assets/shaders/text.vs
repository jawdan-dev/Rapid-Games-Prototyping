#version 460 core

in vec3 v_pos;
in vec2 v_uv;

in float i_index;
in vec3 i_pos;
in float i_scale;
in vec3 i_color;

uniform float u_characterCount;
uniform mat4 u_viewProjection;

out vec2 f_uv;
out vec3 f_color;

void main() {
    gl_Position = u_viewProjection * vec4((v_pos.x * i_scale) + i_pos.x, (v_pos.y * i_scale) + i_pos.y, (v_pos.z * i_scale) + i_pos.z, 1.0);
	f_uv = vec2((v_uv.x + i_index) / u_characterCount, v_uv.y);
	f_color = i_color;
}