#include "core/Window.h"



Window::Window(int width, int height, const std::string& title)
    : m_Width(width), m_Height(height), m_Title(title), m_Window(nullptr) {
        init();
}

void Window::update() {
    glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(m_Window);
    glfwPollEvents();
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(m_Window);
}

GLFWwindow* Window::getNativeWindow() const {
    return m_Window;
}

int Window::getWidth() const {
    return m_Width;
}

int Window::getHeight() const {
    return m_Height;
}

void Window::init() {
    if (!glfwInit) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    //Configure Window
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,6);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(m_Width,m_Height,m_Title.c_str(),nullptr,nullptr);

    if (!m_Window) {
        std::cerr << "Failed to create window" << std::endl;
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(m_Window);

    //initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to Initialise GLAD" << std::endl;
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