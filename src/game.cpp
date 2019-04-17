#include <iostream>
#include <random>
#include <cmath>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"
#include "cubes.h"
#include "noise.h"

void Game::init() {
    cube_program.vertex({"cube.vs"}).fragment({"cube.fs"}).geometry({"cube.gs"}).compile();
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> height(1,3);

    std::vector<Cubes::Instance> cube_instances;
    for (int x = -100; x <= 100; ++x) {
        for (int z = -100; z <= 100; ++z) {
            float f = noise::perlin3d({x * 0.02, 0, z * 0.02}, 3, 0.5) * 10;
            int h = (int)(f + 5);
            for (int y = h; y <= h; ++y) { 
                cube_instances.emplace_back(Cubes::Instance(
                    glm::vec3(x, y, z),
                    glm::vec4(x/100. / 2 + 0.5, z/100. / 2 + 0.5, 0, 1)
                ));
            }
        }
    }

    cubes.vao.instances.set_data(cube_instances);
}

void Game::update() {
    int window_w, window_h;
    glEnable(GL_DEPTH_TEST);
    glfwGetFramebufferSize(window, &window_w, &window_h);
    glViewport(0, 0, window_w, window_h);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(80.f),
        ((float)window_w)/window_h,
        0.1f,
        100.f
    ) * glm::lookAt(
        glm::vec3(sin(glfwGetTime()) * 20, 20, cos(glfwGetTime()) * 20), 
        glm::vec3(0,0,0), 
        glm::vec3(0,1,0)
    );

    cube_program.use();
    glUniformMatrix4fv(cube_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    cubes.draw();
}

