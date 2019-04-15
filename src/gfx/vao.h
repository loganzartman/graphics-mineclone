#pragma once

#include <string>
#include <initializer_list>

#include <glad/glad.h>

#include "vbo.h"

namespace gfx {
class VAO {
public:
    VAO() {}

    VAO& create() {
        glGenVertexArrays(1, &id);
        return *this;
    }

    void destroy() {
        glDeleteVertexArrays(1, &id);
    }

    /**
     * Set the size in bytes of each vertex attribute, in order.
     * Vertex attributes are attributes that are shared by each instance if
     * instanced rendering is used.
     */
    VAO& set_vertex_sizes(std::initializer_list<size_t> sizes) {
        set_layout(sizes, false);
        return *this;
    }

    /**
     * Set the size in bytes of each instanced attribute, in order.
     * Instanced vertex attributes have different values for each instance.
     */
    VAO& set_instance_sizes(std::initializer_list<size_t> sizes) {
        set_layout(sizes, true);
        return *this;
    }

    void bind() {
        if (!id) { create(); }
        glBindVertexArray(id);
    }

    void unbind() {
        glBindVertexArray(0);
    }

    GLuint id = 0;
    VBO vertices;  // non-instanced vertex data
    VBO instances; // per-instance data

private:
    /**
     * Automatically computes glVertexAttribPointer parameters and sets vertex
     * attribute divisor if using instanced rendering.
     */
    void set_layout(std::initializer_list<size_t> sizes, bool instanced) {
        bind();
        if (instanced) { instances.bind(); }
        else { vertices.bind(); }

        size_t stride = 0;
        for (size_t size : sizes) {
            stride += size;
        }

        size_t index = 0;
        size_t offset = 0;
        for (size_t size : sizes) {
            glEnableVertexAttribArray(index);
            // arbitrarily represent all data as floats
            glVertexAttribPointer(index, size, GL_FLOAT, false, stride * sizeof(float), (void*)offset);
            if (instanced) { glVertexAttribDivisor(index, 1); }
            offset += size * sizeof(float);
            ++index;
        }

        unbind();
    }
};
}
