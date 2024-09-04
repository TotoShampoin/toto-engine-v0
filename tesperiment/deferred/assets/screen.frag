#version 460
#define PI 3.14159265359

in vec3 v_position;

out vec4 f_frag_color;

uniform sampler2D u_map;

void main() {
    vec3 color = texture(u_map, v_position.xy).rgb;

    f_frag_color = vec4(color, 1.0);
}
