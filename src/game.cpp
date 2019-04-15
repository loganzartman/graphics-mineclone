#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game.h"
#include "gfx/program.h"
#include "gfx/vao.h"

struct TriInstance {
    TriInstance(glm::vec2 position, glm::vec4 color) : position(position), color(color) {}
    glm::vec2 position;
    glm::vec4 color;
};

void Game::init() {
    demo_program.vertex({"test.vs"}).fragment({"test.fs"}).compile();
    
    // instanced rendering demo: draw two triangles with per-instance position/color
    std::vector<glm::vec3> tri_vertices{
        {-0.5f, -0.5f, 0.0f},
        { 0.5f, -0.5f, 0.0f},
        { 0.0f,  0.5f, 0.0f}
    };
    std::vector<TriInstance> tri_instances{
        TriInstance(glm::vec2(0, 0), glm::vec4(1, 0, 0, 1)),
        TriInstance(glm::vec2(0.2, 0.2), glm::vec4(0, 1, 0, 1))
    };

    // define the layout of the VAO
    // attributes are defined in order 
    // there is a single non-instanced attribute (the vertex position)
    demo_vao.add_attribs({3});
    // there are two attributes for each instance (position, color)
    demo_vao.add_instanced_attribs({2, 4});

    // load the data into its buffers
    demo_vao.vertices.set_data(tri_vertices);
    demo_vao.instances.set_data(tri_instances);
}

void Game::update() {
    int window_w, window_h;
    glfwGetFramebufferSize(window, &window_w, &window_h);
    glViewport(0, 0, window_w, window_h);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    demo_program.use();
    glUniform2i(demo_program.uniform_loc("resolution"), window_w, window_h);
    glUniform1f(demo_program.uniform_loc("time"), glfwGetTime());

    demo_vao.bind();
    glDrawArraysInstanced(GL_TRIANGLES, 0, demo_vao.vertices.size(), demo_vao.instances.size());
}

