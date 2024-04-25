#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform sampler2D u_texture;

void main() {
    vec4 color = texture(u_texture, v_uv);
    f_frag_color = color;
}
