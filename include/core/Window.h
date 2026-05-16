#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

struct GLFWWindowDeleter {
    void operator() (GLFWwindow* window) const {
        if (window) {
            glfwDestroyWindow(window);
        }
    }
};

class Window {
    //CREATE WINDOW

    //The CreateWindow needs to create a window object via a constructor
    //std::unique_ptr<GLFWwindow,GLFWWindowDeleter> window(glfwCreateWindow(800, 600, 
    //"Triangle", nullptr, nullptr));
    /*
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    */

    //MAKE WINDOW CURRENT CONTEXT
    /*

    void printDetails(std::unique_ptr<Weapon>& a) {
        std::cout << a -> Name << " " << a -> Kind << " " << a -> Length << " " << a -> Danger << std::endl;
    }
    */

};