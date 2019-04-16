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
    
    gfx::Program cube_program = gfx::Program("cube");
    gfx::VAO cube_vao;
};
