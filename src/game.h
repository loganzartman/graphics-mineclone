#pragma once

#include <glad/glad.h>

#include "gfx/program.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {};
    void init();
    void update();

    GLFWwindow* window;
    
    gfx::Program demo_program = gfx::Program("demo");
    GLuint demo_vao, demo_vbo;
};

