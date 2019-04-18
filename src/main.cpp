#include <iostream>
#include <sstream>
#include <exception>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "game.h"

bool mouse_locked = false;

/**
 * Called in the event that any GLFW function fails.
 */
void glfw_error_callback(int error, const char* description) {
    std::stringstream str;
    str << "GLFW error " << error << ": " << description;
    throw std::runtime_error(str.str());
}

void
KeyCallback(GLFWwindow* window,
            int key,
            int scancode,
            int action,
            int mods)
{
    Game* game = (Game*)glfwGetWindowUserPointer(window);

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        if (mouse_locked) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            mouse_locked = false;
        }
    }

    if (key == GLFW_KEY_F && mods == GLFW_MOD_CONTROL && action == GLFW_PRESS) {
        game->gravity_switch = !game->gravity_switch;
    }

    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
        game->player_velocity += game->look * 5.f;

    if (action == GLFW_PRESS)
        game->key_pressed[key] = true;
    if (action == GLFW_RELEASE)
        game->key_pressed[key] = false;
}

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{   
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    game->mouse_position = glm::vec2(mouse_x, mouse_y);
}

void
MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (!mouse_locked) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        mouse_locked = true;
    }
}

void FramebufferSizeCallback(GLFWwindow* window, int w, int h) {
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    game->handleResize();
}

int main() {
    // setup glfw
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) { throw std::runtime_error("glfwInit failed"); }

    // create window and GL context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* window = glfwCreateWindow(800, 600, "minecraft", NULL, NULL);
    if (!window) { throw std::runtime_error("glfwCreateWindow failed"); }
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1); // enable vsync (0 to disable)
    Game game(window);
    glfwSetWindowUserPointer(window, &game);
    // Callback functions
    glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MousePosCallback);
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    mouse_locked = true;

    std::cout << "Window creation successful." << std::endl;

    // main loop
    
    game.init();
    while (!glfwWindowShouldClose(window)) {
        game.update();
        glfwSwapBuffers(window);
        glfwPollEvents();

        // collect any GL errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "Uncaught GL error: 0x" << std::hex << err << std::endl;
        }
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

