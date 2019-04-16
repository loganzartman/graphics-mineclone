#include <random>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"
#include "cubes.h"

void Game::init() {
    cube_program.vertex({"cube.vs"}).fragment({"cube.fs"}).geometry({"cube.gs"}).compile();
    
    std::default_random_engine generator;
    std::uniform_int_distribution<int> height(1,3);

    std::vector<Cubes::Instance> cube_instances;
    for (int x = -100; x <= 100; ++x) {
        for (int z = -100; z <= 100; ++z) {
            int h = height(generator);
            for (int y = 0; y < h; ++y) { 
                cube_instances.emplace_back(Cubes::Instance(
                    glm::vec3(x, y, z),
                    glm::vec4(x/5. / 2 + 0.5, z/5. / 2 + 0.5, 0, 1)
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
        glm::radians(100.f),
        ((float)window_w)/window_h,
        0.1f,
        100.f
    ) * glm::lookAt(glm::vec3(0,6,-6), glm::vec3(0,0,0), glm::vec3(0,1,0));

    cube_program.use();
    glUniformMatrix4fv(cube_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    cubes.draw();
}

