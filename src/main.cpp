#include <iostream>
#include <sstream>
#include <exception>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

/**
 * Called in the event that any GLFW function fails.
 */
void glfw_error_callback(int error, const char* description) {
    std::stringstream str;
    str << "GLFW error " << error << ": " << description;
    throw std::runtime_error(str.str());
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

    std::cout << "Window creation successful." << std::endl;

    // main loop
    while (!glfwWindowShouldClose(window)) {
        // todo: game

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}


