#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

void main() {
    f_frag_color = vec4(normalize(v_normal), 1.0);
}
