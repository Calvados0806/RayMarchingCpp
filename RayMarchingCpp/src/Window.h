#pragma once
#include "OpenGL/Renderer.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include <chrono>
#include <iostream>

class Window {
protected:
    Window(GLFWwindow *window, const std::string& title, unsigned int width, unsigned int height);
public:
    using FrameDuration = std::chrono::duration<float>;

    virtual ~Window();

    void Run();
    template <typename WindowType>
    static std::unique_ptr<Window> Create(const std::string& title, unsigned int width = 640, unsigned int height = 480)
    {
        GLFWwindow* window;
        if (!glfwInit()) {
            std::cerr << "glfwInit() failed\n";
            return std::unique_ptr<Window>(nullptr);
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            std::cerr << "glfwCreateWindow() failed\n";
            return std::unique_ptr<Window>(nullptr);
        }

        glfwMakeContextCurrent(window);

        glfwSwapInterval(0);

        if (glewInit() != GLEW_OK) {
            std::cerr << "glewInit() failed\n";
            return std::unique_ptr<Window>(nullptr);
        }
        return std::unique_ptr<Window>(new WindowType(window, title, width, height));
    }
protected:
    virtual bool OnCreate();
    virtual bool OnUpdate(FrameDuration elapsedTime);
protected:
    GLFWwindow* mWindow;
    OpenGL::Renderer mRenderer;
    unsigned int mWidth;
    unsigned int mHeight;
    std::string mTitle;
};