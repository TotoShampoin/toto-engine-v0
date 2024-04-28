#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform sampler2D u_map;
uniform bool u_use_map;

void main() {
    vec3 normal_map = u_use_map ? texture(u_map, v_uv).rgb : vec3(0.5, 0.5, 1.0);
    vec3 normal = normalize(normal_map * 2.0 - 1.0);

    vec3 tangent = normalize(dFdx(v_position));
    vec3 bitangent = normalize(dFdy(v_position));

    mat3 tbn = mat3(tangent, bitangent, v_normal);
    vec3 result = normalize(tbn * normal) * 0.5 + 0.5;

    f_frag_color = vec4(result, 1.0);
}
