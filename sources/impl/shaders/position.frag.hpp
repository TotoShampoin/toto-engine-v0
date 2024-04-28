#pragma once

const char* position_frag = R"frag(
#version 460

in vec3 v_position;

out vec4 f_frag_color;

void main() {
    f_frag_color = vec4(v_position * 0.5 + 0.5, 1.0);
}

)frag";
