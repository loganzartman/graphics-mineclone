#include <iostream>
#include <random>
#include <cmath>

#include <glad/glad.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"
#include "cubes.h"
#include "noise.h"

void Game::init() {
    cube_program.vertex({"cube.vs"}).fragment({"noise.glsl", "cube.fs"}).geometry({"cube.gs"}).compile();
    water_program.vertex({"cube.vs"}).fragment({"water.fs"}).geometry({"cube.gs"}).compile();
    background_tex.set_texture_size(window);
    render_tex.set_texture_size(window);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> height(1,3);

    std::vector<Cubes::Instance> cube_instances;
    std::vector<Cubes::Instance> water_instances;
    for (int x = -100; x <= 100; ++x) {
        for (int z = -100; z <= 100; ++z) {
            float f = noise::perlin3d({x * 0.02, 0, z * 0.02}, 3, 0.5);
            int h = (int)(f * 20);
            for (int y = h - 1; y <= h; ++y) { 
                cube_instances.emplace_back(Cubes::Instance(
                    glm::vec3(x, y, z),
                    (f < 0 ? 1 : f < 0.1 ? 2 : 3)
                ));
            }
            for (int y = h; y <= -5; ++y) {
                water_instances.emplace_back(Cubes::Instance(
                    glm::vec3(x, y, z),
                    4
                ));
            }
        }
    }

    cubes.vao.instances.set_data(cube_instances);
    water_cubes.vao.instances.set_data(water_instances);
}

void Game::update() {
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);

    glm::mat4 view_matrix = glm::lookAt(glm::vec3(player_position), glm::vec3(player_position) + look, up);
    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(80.f),
        ((float)window_w)/window_h,
        0.1f,
        1000.f
    );

    if(moving_forward) {
        player_position += (glm::vec4(look, 0) * forward_direction * movement_speed ); 
    }

    if(moving_sideways) {
        const glm::vec3 tangent = glm::cross(-look, up);
	    player_position +=  (glm::vec4(tangent, 0) * movement_speed * sideways_direction);
    }

    glViewport(0, 0, window_w, window_h);
    glClearColor(0.f,0.f,0.f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    background_tex.bind_framebuffer();
        glEnable(GL_DEPTH_TEST);
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

