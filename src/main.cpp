#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "core/Window.h"
#include <memory>
#include <iostream>


int main() {

    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW\n";
        return -1;
    }

    Window gameWindow(1200,800,"Test Window");

    

    //INPUT

    //UPDATE
    while (!gameWindow.shouldClose()) {
        gameWindow.update();
    }

    //RENDER
    
}