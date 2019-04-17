#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/program.h"
#include "gfx/vao.h"
#include "gfx/rendertexture.h"
#include "cubes.h"
#include "texquad.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {};
    void init();
    void update();
    void updateOrientation();

//  Window, mouse, keyboard attributes
    GLFWwindow* window;
    glm::vec2 mouse_position = glm::vec2(-1,-1);
    glm::vec2 mouse_pos_vector = glm::vec2(0,0);
    bool moving_forward = false;
    bool moving_sideways = false;
    float forward_direction = 1;
    float sideways_direction = 1;
    float movement_speed = 0.5;
    float mouse_speed = 0.003;
    glm::vec4 player_position = glm::vec4(0,7,0,1);
    glm::vec3 look = glm::vec3(0,0,1);
    glm::vec3 up = glm::vec3(0,1,0);

    gfx::Program cube_program = gfx::Program("cube");
    gfx::Program water_program = gfx::Program("cube_water");
    Cubes cubes;
    Cubes water_cubes;

    gfx::RenderTexture background_tex;
    gfx::RenderTexture render_tex;
    TexQuad display_quad;
};
