#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform vec3 u_color;
uniform float u_opacity;
uniform sampler2D u_map;

void main() {
    f_frag_color = texture(u_map, v_uv) * vec4(u_color, u_opacity);
}
