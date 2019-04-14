#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "gfx/program.h"

void Game::init() {
    demo_program.vertex({"test.vs"}).fragment({"test.fs"}).compile();
    
    // add some vertices the old fashioned way
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };
    // create vao
    glGenVertexArrays(1, &demo_vao);
    glBindVertexArray(demo_vao);
    // create vbo
    glGenBuffers(1, &demo_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, demo_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // configure vertex attribs
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

void Game::update() {
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);
    glViewport(0, 0, window_w, window_h);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    demo_program.use();
    glUniform2i(demo_program.uniform_loc("resolution"), window_w, window_h);
    glUniform1f(demo_program.uniform_loc("time"), glfwGetTime());

    glBindVertexArray(demo_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

