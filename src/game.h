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
    void updateOrientation();

//  Window, mouse, keyboard attributes
    GLFWwindow* window;
    glm::vec2 mouse_position = glm::vec2(-1,-1);
    glm::vec2 mouse_pos_vector = glm::vec2(0,0);
    float mouse_speed = 0.003;
    glm::vec4 player_position = glm::vec4(0,5,0,1);
    glm::vec3 look = glm::vec3(0,0,1);
    glm::vec3 up = glm::vec3(0,1,0);

    gfx::Program cube_program = gfx::Program("cube");
    Cubes cubes;
};
