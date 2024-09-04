#version 460

layout(location = 0) in vec3 a_position;

out vec3 v_position;

void main() {
    v_position = a_position * 0.5 + 0.5;
    gl_Position = vec4(a_position, 1.0);
    v_position.y = 1.0 - v_position.y;
    gl_Position.y = -gl_Position.y;
}
