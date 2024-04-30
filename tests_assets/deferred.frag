#version 460
#define PI 3.14159265359

const int LIGHT_MAX = 16;
const int LIGHT_AMBIENT = 0;
const int LIGHT_POINT = 1;
const int LIGHT_DIRECTIONAL = 2;

struct Ligth {
    vec3 color;
    float intensity;
    vec3 pos_or_dir;
    int type;
};

in vec3 v_position;

out vec4 f_frag_color;

uniform sampler2D u_hdri;
uniform sampler2D u_position;
uniform sampler2D u_normal;
uniform sampler2D u_ambient;
uniform sampler2D u_diffuse;
uniform sampler2D u_specular;
uniform sampler2D u_emissive;
uniform sampler2D u_shininess;
uniform sampler2D u_alpha;

uniform mat4 u_view;
uniform mat4 u_projection;

uniform int u_lights_count;
uniform Ligth u_lights[LIGHT_MAX];

// https://en.wikipedia.org/wiki/Blinn%E2%80%93Phong_reflection_model 
vec3 blinnPhong(vec3 light_dir, vec3 light_color, vec3 position, vec3 normal, vec3 diffuse, vec3 specular, float shininess) {
    vec3 camera_dir = normalize(-position);
    vec3 half_vector = normalize(camera_dir + light_dir);
    
    vec3 result_diffuse = diffuse * max(dot(light_dir, normal), 0);
    vec3 result_specular = specular * pow(max(dot(half_vector, normal), 0), shininess);

    return light_color * ( result_diffuse + result_specular );
}

vec3 calculateLight(Ligth light, vec3 ambient, vec3 diffuse, vec3 specular, vec3 position, vec3 normal, float shininess) {
    if (light.type == LIGHT_AMBIENT) {
        return ambient * light.color * light.intensity;
    } else if (light.type == LIGHT_POINT) {
        vec3 direction = normalize(light.pos_or_dir - position);
        float intensity = 1.0 / pow(length(light.pos_or_dir - position), 2);
        return blinnPhong(direction, light.color * light.intensity * intensity, position, normal, diffuse, specular, shininess);
    } else if (light.type == LIGHT_DIRECTIONAL) {
        vec3 direction = normalize(-light.pos_or_dir);
        return blinnPhong(direction, light.color * light.intensity, position, normal, diffuse, specular, shininess);
    } 
    return vec3(0);
}

vec2 directionToUV(vec3 direction) {
    float u = 0.5 + atan(direction.z, direction.x) / (2.0 * PI);
    float v = 0.5 + asin(direction.y) / PI;
    return vec2(u, v);
}

vec4 hdri() {
    vec4 clip_pos = vec4(v_position * 2 - 1, 1.0);
    vec4 eye_pos = inverse(u_projection) * clip_pos;
    eye_pos = vec4(eye_pos.xy, -1.0, 0.0);
    vec4 world_pos = inverse(u_view) * eye_pos;

    vec3 ray = normalize(world_pos.xyz);
    vec2 uv = directionToUV(ray);

    vec3 color = texture(u_hdri, uv).rgb;

    return vec4(color, 1.0);
}

void main() {
    vec3 position = texture(u_position, v_position.xy).xyz;
    vec3 normal = texture(u_normal, v_position.xy).xyz;
    vec3 ambient = texture(u_ambient, v_position.xy).xyz;
    vec3 diffuse = texture(u_diffuse, v_position.xy).xyz;
    vec3 specular = texture(u_specular, v_position.xy).xyz;
    vec3 emissive = texture(u_emissive, v_position.xy).xyz;
    float shininess = texture(u_shininess, v_position.xy).r;
    float alpha = texture(u_alpha, v_position.xy).r;

    vec3 result = emissive;
    for (int i = 0; i < u_lights_count; i++) {
        result += calculateLight(u_lights[i], ambient, diffuse, specular, position, normal, shininess);
    }
    result = clamp(result, 0, 1);

    f_frag_color = hdri() * (1 - alpha) + vec4(result, alpha) * alpha;
}
