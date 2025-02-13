#version 460 core

in vec3 v_pos;
in float test;
in vec3 anything;

out vec3 colorOffset;

void main() {
    gl_Position = vec4(v_pos.x + test, v_pos.y, v_pos.z, 1.0);
	colorOffset = anything;
}