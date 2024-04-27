#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

void main() {
    f_frag_color = vec4(vec3(gl_FragCoord.z), 1.0);
}
