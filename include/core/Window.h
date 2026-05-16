#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

class Window {
 
    public:
        Window(int width, int height, const std::string& title);
        
        void update();

        bool shouldClose() const;

        GLFWwindow* getNativeWindow() const;

        int getHeight() const;
        int getWidth() const;

    private:
        int m_Width;
        int m_Height;
        std::string m_Title;
        GLFWwindow* m_Window;

        void init();

};