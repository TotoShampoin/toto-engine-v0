#include <GL/glew.h>
#include <TotoEngine/TotoEngine.hpp>

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>

#include "res/shaders/vertex.glsl.hpp"
#include "res/shaders/fragment.glsl.hpp"


int main(int /* argc */, const char* /* argv */[]) {
    TotoEngine::printInfo();
    TotoEngine::Window::init();

    TotoEngine::Window window(800, 600, "TotoEngine");
    window.makeContextCurrent();

    glewInit();

    struct vec2 {
        float x, y;
    };
    struct vec3 {
        float x, y, z;
    };
    struct Vertex {
        vec3 position;
        vec3 normal;
        vec2 uv;
    };
    static constexpr auto VERTEX_ATTRIBUTE_POSITION = 0;
    static constexpr auto VERTEX_ATTRIBUTE_NORMAL = 1;
    static constexpr auto VERTEX_ATTRIBUTE_UV = 2;
    
    window.makeContextCurrent();
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    auto vertices = std::vector<Vertex> {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f}}
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    GLuint ibo;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    auto indices = std::vector<GLuint>{
        0, 1, 2,
        2, 3, 0
    };
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_NORMAL);
    glEnableVertexAttribArray(VERTEX_ATTRIBUTE_UV);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(VERTEX_ATTRIBUTE_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(VERTEX_ATTRIBUTE_NORMAL, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glVertexAttribPointer(VERTEX_ATTRIBUTE_UV, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    {
        glShaderSource(vertex_shader, 1, &vertex, nullptr);
        glCompileShader(vertex_shader);
        int success;
        char info_log[512];
        glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << info_log << std::endl;
        }
    }
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    {
        glShaderSource(fragment_shader, 1, &fragment, nullptr);
        glCompileShader(fragment_shader);
        int success;
        char info_log[512];
        glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << info_log << std::endl;
        }
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    {
        int success;
        char info_log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(program, 512, nullptr, info_log);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << info_log << std::endl;
        }
    }
    glUseProgram(program);

    auto projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.1f, 100.0f);
    auto view = glm::mat4(1.0f);
    auto model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));

    auto u_projection = glGetUniformLocation(program, "u_projection");
    auto u_view = glGetUniformLocation(program, "u_view");
    auto u_model = glGetUniformLocation(program, "u_model");

    glUniformMatrix4fv(u_projection, 1, GL_FALSE, &projection[0][0]);
    glUniformMatrix4fv(u_view, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);
    while(!window.shouldClose()) {
        window.makeContextCurrent();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glBindVertexArray(vao);
        glUseProgram(program);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

        model = glm::rotate(model, glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(u_model, 1, GL_FALSE, &model[0][0]);

        window.swapBuffers();
        window.pollEvents();
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);

    glDeleteBuffers(1, &ibo);
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);

    return 0;
}
