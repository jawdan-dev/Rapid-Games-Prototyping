#version 460 core

in vec3 v_pos;
in float test;
in float test2;
in vec3 anything;

uniform mat4 u_viewProjection;

out vec3 colorOffset;

void main() {
    gl_Position = u_viewProjection * vec4(v_pos.x + test, v_pos.y + test2, v_pos.z, 1.0);
	colorOffset = anything;
}