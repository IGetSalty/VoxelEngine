#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

class Window {
public:
    Window(int width, int height, const std::string& title)
        : m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr) {
        init();
    }

    ~Window() {
        glfwDestroyWindow(m_Window);
        glfwTerminate();
    }

    void update() {
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    bool shouldClose() const {
        return glfwWindowShouldClose(m_Window);
    }

    // Getters
    GLFWwindow* getNativeWindow() const { return m_Window; }
    int getWidth() const { return m_Width; }
    int getHeight() const { return m_Height; }

private:
    int m_Width;
    int m_Height;
    std::string m_Title;
    GLFWwindow* m_Window;

    void init() {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Configure GLFW for OpenGL
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        if (!m_Window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            exit(EXIT_FAILURE);
        }

        glfwMakeContextCurrent(m_Window);
        
        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            std::cerr << "Failed to initialize GLAD" << std::endl;
            exit(EXIT_FAILURE);
        }

        // Setup resize callback
        glfwSetWindowUserPointer(m_Window, this);
        glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
            Window* win = static_cast<Window*>(glfwGetWindowUserPointer(window));
            win->m_Width = width;
            win->m_Height = height;
            glViewport(0, 0, width, height);
        });
    }
};