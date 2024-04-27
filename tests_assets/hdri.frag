#version 460
#define PI 3.14159265359

in vec3 v_position;

out vec4 f_frag_color;

uniform mat4 u_projection;
uniform mat4 u_view;

uniform sampler2D u_map;

vec2 directionToUV(vec3 direction) {
    float u = 0.5 + atan(direction.z, direction.x) / (2.0 * PI);
    float v = 0.5 + asin(direction.y) / PI;
    return vec2(u, v);
}

void main() {
    vec4 clip_pos = vec4(v_position, 1.0);
    vec4 eye_pos = inverse(u_projection) * clip_pos;
    eye_pos = vec4(eye_pos.xy, -1.0, 0.0);
    vec4 world_pos = inverse(u_view) * eye_pos;

    vec3 ray = normalize(world_pos.xyz);
    vec2 uv = directionToUV(ray);

    vec3 color = texture(u_map, uv).rgb;

    f_frag_color = vec4(color, 1.0);
}
