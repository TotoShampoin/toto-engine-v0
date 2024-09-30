#version 460

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

out vec3 v_world_position;
out vec3 v_position;
out vec3 v_world_normal;
out vec3 v_normal;
out vec2 v_uv;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
    mat3 normal_matrix = mat3(transpose(inverse(u_view * u_model)));
    
    vec4 world_position = u_model * vec4(a_position, 1.);
    vec4 view_position = u_view * world_position;
    vec4 clip_position = u_projection * view_position;
    vec4 world_normal = u_model * vec4(a_normal, 0.);
    vec3 normal = normal_matrix * a_normal;

    v_world_position = world_position.xyz;
    v_position = view_position.xyz;
    v_world_normal = world_normal.xyz;
    v_normal = normal;
    v_uv = a_uv;

    gl_Position = clip_position;
}
