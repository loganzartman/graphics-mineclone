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
    render_tex.set_texture_size(window);
    display_quad.set_texture(render_tex.color_id);
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> height(1,3);

    std::vector<Cubes::Instance> cube_instances;
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
                cube_instances.emplace_back(Cubes::Instance(
                    glm::vec3(x, y, z),
                    4
                ));
            }
        }
    }

    cubes.vao.instances.set_data(cube_instances);
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
    ) * view_matrix;

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

    render_tex.bind_framebuffer();
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glViewport(0, 0, window_w, window_h);
        glClearColor(0.2f,0.2f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cube_program.use();
        glUniformMatrix4fv(cube_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
        cubes.draw();
    render_tex.unbind_framebuffer();

    display_quad.draw();
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

