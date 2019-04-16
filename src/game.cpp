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
    
    std::vector<Cubes::Instance> cube_instances{
        Cubes::Instance(glm::vec3(0, 0, 0), glm::vec4(1,0,0,1)),
        Cubes::Instance(glm::vec3(1.8, 0, 0), glm::vec4(0,1,0,1))
    };

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
    ) * glm::lookAt(glm::vec3(0,3,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));

    cube_program.use();
    glUniformMatrix4fv(cube_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    cubes.draw();
}

