#include <iostream>
#include <random>
#include <cmath>
#include <array>

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/projection.hpp>
#include <iostream>
#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"
#include "cubes.h"
#include "noise.h"
#include "world.h"

void Game::init() {
    cube_program.vertex({"cube.vs"}).fragment({"noise.glsl", "cube.fs"}).geometry({"cube.gs"}).compile();
    water_program.vertex({"cube.vs"}).fragment({"water.fs"}).geometry({"cube.gs"}).compile();
    handleResize();
}

void Game::handleResize() {
    background_tex.set_texture_size(window);
    render_tex.set_texture_size(window);
}

void Game::update() {
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);

    world.load_nearby(player_position);
    world.unload_far(player_position);
    world.update_cubes_instances();

    const glm::vec3 camera_position = player_position + glm::vec3(0, 0.5, 0);
    glm::mat4 view_matrix = glm::lookAt(camera_position, camera_position + look, up);
    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(80.f),
        ((float)window_w)/window_h,
        0.1f,
        1000.f
    );

    bool first = mouse_prev == glm::vec2(-1,-1);
    if (first) {
        mouse_prev = mouse_position;
    } else {
        mouse_pos_vector = -(mouse_position - mouse_prev);
        updateOrientation();
        mouse_prev = mouse_position;
    }

    if (key_pressed[GLFW_KEY_SPACE] && on_ground) {
        player_velocity += glm::vec3(0, 1, 0);
        on_ground = false;
    }

    const glm::vec3 tangent = glm::cross(-look, up);
    bool any_key = false;
    if (key_pressed[GLFW_KEY_W]) {
        applyInputAccel(look * acceleration);
        any_key = true;
    }
    if (key_pressed[GLFW_KEY_S]) {
        applyInputAccel(look * (-acceleration));
        any_key = true;
    }
    if (key_pressed[GLFW_KEY_D]) {
        applyInputAccel(tangent * (-acceleration));
        any_key = true;
    }
    if (key_pressed[GLFW_KEY_A]) {
        applyInputAccel(tangent * acceleration);
        any_key = true;
    } 
    if (!any_key) {
        player_motion *= 0.8;
    }

    if (gravity_switch) {
        player_velocity += (-up) * gravity;
    }

    glm::vec3 movement = player_motion + player_velocity;
    int steps = glm::length(movement) * 500;
    glm::vec3 step = movement / (float)steps;
    // std::cout << steps << std::endl;

    const std::array<glm::vec3, 3> directions{
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(0, 0, 1)
    };

    for (int i = 0; i < steps; ++i) {
        const glm::ivec3 old_grid_pos = gridWorld(player_position);
        for (const glm::vec3& dir : directions) {
            glm::vec3 dstep = dir * step;
            const glm::ivec3 grid_pos_feet = gridWorld(dstep + player_position + glm::vec3(0,-1,0));
            const glm::ivec3 grid_pos = gridWorld(dstep + player_position);

            if (world.is_solid(grid_pos) || world.is_solid(grid_pos_feet)) {
                if (grid_pos != old_grid_pos) {
                    // moved
                    const glm::vec3 hit_normal = glm::normalize(glm::vec3(old_grid_pos - grid_pos));
                    if (hit_normal.y > 0.9) {
                        on_ground = true;
                    }
                    step -= glm::proj(step, hit_normal);
                    dstep -= glm::proj(dstep, hit_normal);
                    player_velocity -= glm::proj(player_velocity, hit_normal);
                } else {
                    // already in block
                    // std::cout << "resolving collision" << std::endl;
                    glm::vec3 dx = player_position - (glm::vec3(grid_pos) + glm::vec3(0.5));
                    player_position += dx;
                }
            }
            player_position += dstep;
        }
    }

    player_velocity *= 0.98;

    glViewport(0, 0, window_w, window_h);
    glClearColor(0.f,0.f,0.f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    background_tex.bind_framebuffer();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, window_w, window_h);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube_program.use();
        glUniformMatrix4fv(cube_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
        glUniformMatrix4fv(cube_program.uniform_loc("view"), 1, false, glm::value_ptr(view_matrix));
        cubes.draw();
    background_tex.unbind_framebuffer();

    glEnable(GL_DEPTH_TEST);
    display_quad.set_texture(background_tex.color_id).set_depth(background_tex.depth_id).draw();

    water_program.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, background_tex.color_id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, background_tex.depth_id);

    glUniform1i(water_program.uniform_loc("background"), 0);
    glUniform1i(water_program.uniform_loc("depth"), 1);
    glUniform1f(water_program.uniform_loc("z_near"), 0.1f);
    glUniform1f(water_program.uniform_loc("z_far"), 1000.f);
    glUniform1f(water_program.uniform_loc("time"), glfwGetTime());
    glUniform3fv(water_program.uniform_loc("look"), 1, glm::value_ptr(look));
    glUniformMatrix4fv(water_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    glUniformMatrix4fv(water_program.uniform_loc("view"), 1, false, glm::value_ptr(view_matrix));
    glUniform2i(water_program.uniform_loc("resolution"), window_w, window_h);
    
    water_cubes.draw();
}

void Game::updateOrientation() {
    if (glm::length(mouse_pos_vector) == 0) {
		return;}
	mouse_pos_vector.x *= -1.f;
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);
     glm::mat4 projection_matrix = glm::perspective(
        glm::radians(80.f),
        ((float)window_w)/window_h,
        0.1f,
        100.f
    );

    glm::mat4 view_matrix = glm::lookAt(glm::vec3(player_position), glm::vec3(player_position) + look, up);

	const glm::vec3 world_vector = glm::vec3(glm::inverse(projection_matrix * view_matrix) * glm::vec4(mouse_pos_vector, 1., 1.));
	const glm::vec3 rotation_axis = glm::cross(look, world_vector);
	const glm::mat4 rotation = glm::rotate(mouse_speed * glm::length(mouse_pos_vector), rotation_axis);
	look = glm::vec3(rotation * glm::vec4(look, 0.));
}

glm::ivec3 Game::gridWorld(const glm::vec3& pos) {
    return glm::ivec3((int)(pos.x), (int)(pos.y), (int)(pos.z));
}

void Game::applyInputAccel(const glm::vec3& acc) {
    const glm::vec3 new_vel = player_motion + acc;
    const float new_speed = std::min(max_speed, glm::length(new_vel));
    player_motion = glm::normalize(new_vel) * new_speed;
}
