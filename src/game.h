#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "gfx/program.h"
#include "gfx/vao.h"
#include "gfx/rendertexture.h"
#include "cubes.h"
#include "block.h"
#include "texquad.h"
#include "world.h"

struct Game {
    Game(GLFWwindow* window) : window(window) {}
    void init();
    void handleResize();
    void update();
    void updateOrientation();
    bool intersectWorld(const glm::vec4& new_position);
    glm::ivec3 gridWorld(const glm::vec3& pos);
    void applyInputAccel(const glm::vec3& acc);

//  Window, mouse, keyboard attributes
    GLFWwindow* window;
    std::unordered_map<int, bool> key_pressed;
    glm::vec2 mouse_position = glm::vec2(-1,-1);
    glm::vec2 mouse_prev = glm::vec2(-1,-1);
    glm::vec2 mouse_pos_vector = glm::vec2(0,0);
    bool gravity_switch = false;
    bool on_ground = false;
    float acceleration = 0.025;
    float gravity = 0.05;
    float max_speed = 0.2;
    float mouse_speed = 0.3;
    float pitch = 0;
    float yaw = 0;
    glm::vec3 player_position = glm::vec3(38,28,45); // negative chunk indices don't load so well rn
    glm::vec3 player_velocity = glm::vec3(0,0,0);
    glm::vec3 player_motion = glm::vec3(0,0,0);
    glm::vec3 look = glm::vec3(0,0,1);
    glm::vec3 up = glm::vec3(0,1,0);

    gfx::Program cube_program = gfx::Program("cube");
    gfx::Program water_program = gfx::Program("cube_water");
    Cubes cubes;
    Cubes water_cubes;

    gfx::RenderTexture background_tex;
    gfx::RenderTexture render_tex;
    TexQuad display_quad;

    World world = World(cubes, water_cubes);
};
