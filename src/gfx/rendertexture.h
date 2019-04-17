#pragma once

#include <glad/glad.h>

namespace gfx {
class RenderTexture {
private:
    glm::ivec2 tex_dim;

public:
    RenderTexture() {}
    
    void create() {
        glGenFramebuffers(1, &fbo_id);
        glGenTextures(1, &color_id);
        glGenTextures(1, &depth_id);
    }

    void destroy() {
        glDeleteTextures(1, &depth_id);
        glDeleteTextures(1, &color_id);
        glDeleteFramebuffers(1, &fbo_id);
    }

    /**
     * @brief Bind framebuffer for rendering to texture
     */
    void bind_framebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo_id);
    }

    /**
     * @brief Unbind framebuffer for resetting render target
     */
    void unbind_framebuffer() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void set_texture_size(GLFWwindow* window) {
        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        set_texture_size(w, h);
    }

    void set_texture_size(int w, int h) {
        if (!fbo_id) { create(); }
        if (w != tex_dim.x || h != tex_dim.y) {
            tex_dim = {w, h};
            bind_framebuffer();
            glBindTexture(GL_TEXTURE_2D, color_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glBindTexture(GL_TEXTURE_2D, depth_id);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // attach texture to framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, color_id, 0);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_id, 0);

            if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

            unbind_framebuffer();
        }
    }

    GLuint fbo_id = 0;
    GLuint color_id = 0, depth_id = 0;
};
}