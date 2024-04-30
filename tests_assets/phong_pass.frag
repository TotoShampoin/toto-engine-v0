#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

layout(location = 0) out vec4 f_position;
layout(location = 1) out vec4 f_normal;
layout(location = 2) out vec4 f_ambient;
layout(location = 3) out vec4 f_diffuse;
layout(location = 4) out vec4 f_specular;
layout(location = 5) out vec4 f_emissive;
layout(location = 6) out float f_shininess;
layout(location = 7) out float f_alpha;

// out vec4 f_frag_color;

uniform vec3 u_ambient;
uniform vec3 u_diffuse;
uniform vec3 u_specular;
uniform vec3 u_emissive;
uniform float u_shininess;
uniform float u_opacity;
uniform sampler2D u_ambient_map;
uniform sampler2D u_diffuse_map;
uniform sampler2D u_specular_map;
uniform sampler2D u_emissive_map;
uniform sampler2D u_shininess_map;
uniform sampler2D u_opacity_map;
uniform bool u_use_ambient_map;
uniform bool u_use_diffuse_map;
uniform bool u_use_specular_map;
uniform bool u_use_emissive_map;
uniform bool u_use_shininess_map;
uniform bool u_use_opacity_map;

void main() {
    vec3 ambient = u_ambient;
    vec3 diffuse = u_diffuse;
    vec3 specular = u_specular;
    vec3 emissive = u_emissive;
    float shininess = u_shininess;
    float opacity = u_opacity;

    if (u_use_ambient_map) {
        ambient *= texture(u_ambient_map, v_uv).rgb;
    }
    if (u_use_diffuse_map) {
        diffuse *= texture(u_diffuse_map, v_uv).rgb;
    }
    if (u_use_specular_map) {
        specular *= texture(u_specular_map, v_uv).rgb;
    }
    if (u_use_emissive_map) {
        emissive *= texture(u_emissive_map, v_uv).rgb;
    }
    if (u_use_shininess_map) {
        shininess *= texture(u_shininess_map, v_uv).r;
    }
    if (u_use_opacity_map) {
        opacity *= texture(u_opacity_map, v_uv).r;
    }

    vec3 position = v_position;
    vec3 normal = normalize(v_normal);

    f_position = vec4(position, 1);
    f_normal = vec4(normal, 1);
    f_ambient = vec4(ambient, 1);
    f_diffuse = vec4(diffuse, 1);
    f_specular = vec4(specular, 1);
    f_emissive = vec4(emissive, 1);
    f_shininess = shininess;
    f_alpha = opacity;
}
