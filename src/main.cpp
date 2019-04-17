#include <iostream>
#include <sstream>
#include <exception>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtx/string_cast.hpp>

#include "game.h"

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

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void
MousePosCallback(GLFWwindow* window, double mouse_x, double mouse_y)
{   
    Game* game = (Game*)glfwGetWindowUserPointer(window);
    bool first = game->mouse_position == glm::vec2(-1,-1);
    float dx = mouse_x - game->mouse_position.x;
    float dy = mouse_y - game->mouse_position.y;
    game->mouse_position = glm::vec2(mouse_x, mouse_y);
    game->mouse_pos_vector = -glm::vec2(dx,dy);
    if (first) return;

    game->updateOrientation();

}

void
MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	return;
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

