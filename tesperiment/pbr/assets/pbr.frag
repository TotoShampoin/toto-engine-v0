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
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform vec3 u_albedo;
uniform sampler2D u_albedo_map;
uniform bool u_use_albedo_map;

uniform float u_metallic;
uniform sampler2D u_metallic_map;
uniform bool u_use_metallic_map;

uniform float u_roughness;
uniform sampler2D u_roughness_map;
uniform bool u_use_roughness_map;

uniform vec3 u_emissive;
uniform sampler2D u_emissive_map;
uniform bool u_use_emissive_map;

uniform int u_lights_count;
uniform Ligth u_lights[LIGHT_MAX];

uniform mat4 u_view;

uniform sampler2D u_environment;
uniform sampler2D u_irradiance;

float distributionGGX(float NdotH, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH2 = NdotH * NdotH;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    return a2 / (PI * denom * denom);
}

float geometrySchlickGGX(float NdotV, float k) {
    return NdotV / (NdotV * (1.0 - k) + k);
}
float geometrySmith(float NdotV, float NdotL, float roughness) {
    return geometrySchlickGGX(NdotV, roughness) * geometrySchlickGGX(NdotL, roughness);
}

float fresnelSchlick(float HdotV, float F0) {
    return F0 + (1.0 - F0) * pow(1.0 - HdotV, 5);
}

vec2 directionToUV(vec3 direction) {
    float u = 0.5 + atan(direction.z, direction.x) / (2.0 * PI);
    float v = 0.5 + asin(direction.y) / PI;
    return vec2(u, v);
}


vec3 pbr(vec3 albedo, float metallic, float roughness, vec3 normal_dir, vec3 view_dir, vec3 light_dir, vec3 light_diffuse, vec3 light_specular) {
    float diffuse = max(dot(normal_dir, light_dir), 0);

    vec3 half_vector = normalize(light_dir + view_dir);
    float NdotH = max(dot(normal_dir, half_vector), 0);
    float NdotL = max(dot(normal_dir, light_dir), 0);
    float NdotV = max(dot(normal_dir, view_dir), 0);
    float HdotV = max(dot(half_vector, view_dir), 0);

    float D = distributionGGX(NdotH, roughness);
    float G = geometrySmith(NdotV, NdotL, roughness);
    float F = fresnelSchlick(HdotV, 0.04);

    float specular = D * F * G / (4 * NdotL * NdotV);

    return albedo * diffuse * (1.0 - metallic) * light_diffuse + specular * mix(vec3(1), albedo, metallic) * light_specular;
}

vec3 calculateLight(Ligth light, vec3 position, vec3 normal, vec3 albedo, float metallic, float roughness) {
    if (light.type == LIGHT_AMBIENT) {
        return albedo * light.color * light.intensity;
    } else if (light.type == LIGHT_POINT) {
        vec3 direction = normalize(light.pos_or_dir - position);
        float intensity = 1.0 / pow(length(light.pos_or_dir - position), 2);
        vec3 light_color = light.color * light.intensity * intensity;
        return pbr(albedo, metallic, roughness, normal, normalize(-position), direction, light_color, light_color);
    } else if (light.type == LIGHT_DIRECTIONAL) {
        vec3 direction = normalize(-light.pos_or_dir);
        vec3 light_color = light.color * light.intensity;
        return pbr(albedo, metallic, roughness, normal, normalize(-position), direction, light_color, light_color);
    } 
    return vec3(0);
}

void main() {
    vec3 albedo = u_use_albedo_map ? texture(u_albedo_map, v_uv).rgb : u_albedo;
    float metallic = u_use_metallic_map ? texture(u_metallic_map, v_uv).r : u_metallic;
    float roughness = u_use_roughness_map ? texture(u_roughness_map, v_uv).r : u_roughness;
    vec3 emissive = u_use_emissive_map ? texture(u_emissive_map, v_uv).rgb : u_emissive;
    float alpha = texture(u_albedo_map, v_uv).a;

    vec3 result = emissive;
    for (int i = 0; i < u_lights_count; i++) {
        result += calculateLight(u_lights[i], v_position, normalize(v_normal), albedo, metallic, roughness);
    }
    // vec3 world_cam = (inverse(u_view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
    // vec3 world_normal = (inverse(u_view) * vec4(v_normal, 0.0)).xyz;
    // vec3 world_position = (inverse(u_view) * vec4(v_position, 1.0)).xyz;
    // vec4 env_diffuse_rgbm = texture(u_irradiance, directionToUV(world_normal));
    // vec3 env_diffuse = env_diffuse_rgbm.rgb * env_diffuse_rgbm.a * 10.0;
    // vec3 env_specular = texture(u_environment, directionToUV(reflect(normalize(world_position - world_cam), world_normal))).rgb;
    //TODO: Implement IBL

    f_frag_color = vec4(result, alpha);
}