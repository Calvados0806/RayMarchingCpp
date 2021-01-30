#pragma once
#include "OpenGL/Renderer.h"
#include "OpenGL/GLCore.h"

#include <GLFW/glfw3.h>

#include <string>
#include <memory>
#include <chrono>
#include <iostream>

class Window {
public:
    using FrameDuration = std::chrono::duration<float>;

    virtual ~Window();
    void Run();
    virtual void OnKeyEvent(int key, int action, int mods);
private:
    Window(GLFWwindow *window, const std::string& title, unsigned int width, unsigned int height);
protected:
    template <typename WindowType>
    static WindowType* Create(const std::string& title, unsigned int width = 640, unsigned int height = 480)
    {
        static bool sIsContextInitialized = false;

        if (!sIsContextInitialized) {
            ASSERT(glfwInit());
            sIsContextInitialized = true;
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        GLFWwindow* window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        ASSERT(window);

        glfwMakeContextCurrent(window);

        glfwSwapInterval(0);

        ASSERT(glewInit() == GLEW_OK);
        return new WindowType(window, title, width, height);
    }

    virtual bool OnCreate();
    virtual bool OnUpdate(FrameDuration elapsedTime);
    virtual void OnImGuiUpdate();

    GLFWwindow* mWindow;
    OpenGL::Renderer mRenderer;
    unsigned int mWidth;
    unsigned int mHeight;
    std::string mTitle;
};