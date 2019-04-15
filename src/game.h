#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/program.h"
#include "gfx/vao.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {};
    void init();
    void update();

    GLFWwindow* window;
    
    gfx::Program demo_program = gfx::Program("demo");
    gfx::VAO demo_vao;
};
