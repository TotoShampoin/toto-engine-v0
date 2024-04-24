#pragma once

const char* fragment = R"glsl(
#version 460

in vec3 v_position;
in vec3 v_normal;
in vec2 v_uv;

out vec4 f_frag_color;

void main() {
    f_frag_color = vec4(v_uv, 0.0, 1.0);
}

)glsl";
