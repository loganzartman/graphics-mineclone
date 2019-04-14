#pragma once

#include <glad/glad.h>

#include "gfx/program.h"
#include "gfx/vbo.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {};
    void init();
    void update();

    GLFWwindow* window;
    
    gfx::Program demo_program = gfx::Program("demo");
    gfx::VBO demo_vbo;
    GLuint demo_vao;
};

