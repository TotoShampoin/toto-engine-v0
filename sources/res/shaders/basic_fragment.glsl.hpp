#pragma once

const char* basic_fragment = R"glsl(
#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

uniform vec3 u_color;
uniform float u_opacity;
uniform sampler2D u_map;
uniform bool u_use_map;

void main() {
    vec4 map = u_use_map ? texture(u_map, v_uv) : vec4(1.0);
    f_frag_color = map * vec4(u_color, u_opacity);
}

)glsl";
