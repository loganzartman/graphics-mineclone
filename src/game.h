#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/program.h"
#include "gfx/vao.h"
#include "cubes.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {};
    void init();
    void update();

    GLFWwindow* window;
    glm::vec2 mouse_position;
    
    gfx::Program cube_program = gfx::Program("cube");
    Cubes cubes;
};
