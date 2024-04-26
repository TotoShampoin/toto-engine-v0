#version 460

const int LIGHT_MAX = 16;
const int LIGHT_AMBIENT = 0;
const int LIGHT_POINT = 1;
const int LIGHT_DIRECTIONAL = 2;

struct Ligth {
    vec3 color;
    float strength;
    vec3 pos_or_dir;
    int type;
};

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

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

uniform int u_lights_count;
uniform Ligth u_lights[LIGHT_MAX];

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model 
vec3 blinnPhong(vec3 light_dir, vec3 light_color, vec3 normal, vec3 diffuse, vec3 specular, float shininess) {
    vec3 camera_dir = normalize(-v_position);
    vec3 half_vector = normalize(camera_dir + light_dir);
    
    vec3 result_diffuse = diffuse * max(dot(light_dir, normal), 0);
    vec3 result_specular = specular * pow(max(dot(half_vector, normal), 0), shininess);

    return light_color * ( result_diffuse + result_specular );
}

vec3 calculateLight(Ligth light, vec3 ambient, vec3 diffuse, vec3 specular, vec3 normal, float shininess) {
    if (light.type == LIGHT_AMBIENT) {
        return ambient * light.color * light.strength;
    } else if (light.type == LIGHT_POINT) {
        vec3 direction = normalize(light.pos_or_dir - v_position);
        float strength = 1.0 / pow(length(light.pos_or_dir - v_position), 2);
        return blinnPhong(direction, light.color * light.strength * strength, normal, diffuse, specular, shininess);
    } else if (light.type == LIGHT_DIRECTIONAL) {
        vec3 direction = normalize(-light.pos_or_dir);
        return blinnPhong(direction, light.color * light.strength, normal, diffuse, specular, shininess);
    } 
    return vec3(0);
}

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

    vec3 normal = normalize(v_normal);
    vec3 color = emissive;
    for(int i = 0; i < u_lights_count; i++) {
        color += calculateLight(u_lights[i], ambient, diffuse, specular, normal, shininess);
    }

    f_frag_color = vec4(color, opacity);
}
