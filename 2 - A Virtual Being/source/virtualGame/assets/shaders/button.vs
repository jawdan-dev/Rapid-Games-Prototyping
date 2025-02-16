#version 460 core

in vec3 v_pos;
in vec2 i_pos;
in vec3 i_col;
in vec2 i_scale;

uniform mat4 u_viewProjection;

out vec3 f_col;

void main() {
    gl_Position = u_viewProjection * vec4(
		(v_pos.x * i_scale.x) + i_pos.x,
		(v_pos.y * i_scale.y) + i_pos.y,
		v_pos.z,
		1.0f
	);
	f_col = i_col;
}