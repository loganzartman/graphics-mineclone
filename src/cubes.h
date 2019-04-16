#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "gfx/vao.h"

struct Cubes {
    Cubes() {
        // define the layout of the VAO
        // attributes are defined in order 
        // there is a single non-instanced attribute (the vertex position)
        vao.add_attribs({3});
        // there are two attributes for each instance (position, color)
        vao.add_instanced_attribs({3, 4});

        // load the vertex data 
        vao.vertices.set_data(cube_vertices);
    }

    void draw() {
        vao.bind();
        glDrawElementsInstanced(GL_TRIANGLES, cube_indices.size() * 3, GL_UNSIGNED_INT, cube_indices.data(), vao.instances.size());
        vao.unbind();
    }

    gfx::VAO vao;

    struct Instance {
        Instance(glm::vec3 position, glm::vec4 color) : position(position), color(color) {}
        glm::vec3 position;
        glm::vec4 color;
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
};
