#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"

struct TriInstance {
    TriInstance(glm::vec2 position, glm::vec4 color) : position(position), color(color) {}
    glm::vec2 position;
    glm::vec4 color;
};

struct CubeInstance {
    CubeInstance(glm::vec3 position, glm::vec4 color) : position(position), color(color) {}
    glm::vec3 position;
    glm::vec4 color;
};

void Game::init() {
    cube_program.vertex({"cube.vs"}).fragment({"cube.fs"}).geometry({"cube.gs"}).compile();

    std::vector<CubeInstance> cube_instances{
        CubeInstance(glm::vec3(0, 0, 0), glm::vec4(1,0,0,1)),
        CubeInstance(glm::vec3(1.8, 0, 0), glm::vec4(0,1,0,1))
    };

    std::vector<glm::vec3> cube_vertices{
        {0.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 0.0f},
        {1.0f, 0.0f, 1.0f},
        {0.0f, 0.0f, 1.0f},
        {0.0f, 1.0f, 1.0f},
        {0.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 0.0f},
        {1.0f, 1.0f, 1.0f}

    };


    // define the layout of the VAO
    // attributes are defined in order 
    // there is a single non-instanced attribute (the vertex position)
    // there are two attributes for each instance (position, color)

    // load the data into its buffers

    cube_vao.add_attribs({3});
    cube_vao.add_instanced_attribs({3, 4});
    cube_vao.vertices.set_data(cube_vertices);
    cube_vao.instances.set_data(cube_instances);

}

void Game::update() {
    int window_w, window_h;
    glEnable(GL_DEPTH_TEST);
    glfwGetFramebufferSize(window, &window_w, &window_h);
    glViewport(0, 0, window_w, window_h);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      std::vector<glm::uvec3> cube_indices{
        {0, 1, 2},
        {2, 3, 0},
        {0, 5, 4},
        {4, 3, 0},
        {0, 5, 6},
        {6, 1, 0},
        {5, 4, 7},
        {7, 6, 5},
        {1, 2, 7},
        {7, 6, 1},
        {3, 2, 7},
        {7, 4, 3}
    };

    glm::mat4 projection_matrix = glm::perspective(
        glm::radians(100.f),
        ((float)window_w)/window_h,
        0.1f,
        100.f
    ) * glm::lookAt(glm::vec3(0,3,-3), glm::vec3(0,0,0), glm::vec3(0,1,0));

    cube_program.use();
    glUniformMatrix4fv(cube_program.uniform_loc("projection"), 1, false, glm::value_ptr(projection_matrix));
    cube_vao.bind();
    glDrawElementsInstanced(GL_TRIANGLES, cube_indices.size() * 3, GL_UNSIGNED_INT, cube_indices.data(), cube_vao.instances.size());
}

