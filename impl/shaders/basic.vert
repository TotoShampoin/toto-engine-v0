#version 460

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_position;
out vec3 v_normal;
out vec2 v_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    mat3 normal_matrix = mat3(transpose(inverse(u_view * u_model)));
    
    vec4 position = u_view * u_model * vec4(a_position, 1.);
    vec3 normal = normal_matrix * a_normal;

    v_position = position.xyz;
    v_normal = normal;
    v_uv = a_uv;

    gl_Position = u_projection * position;
}
