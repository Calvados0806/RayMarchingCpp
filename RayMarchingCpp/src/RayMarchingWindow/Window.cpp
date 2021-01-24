#include "Window.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

Window::Window(GLFWwindow *window, const std::string& title, unsigned int width, unsigned int height) :
    mWindow(window), mRenderer(), mWidth(width), mHeight(height), mTitle(title)
{
    glfwSetWindowUserPointer(mWindow, this);
    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        Window* w = static_cast<Window*>(glfwGetWindowUserPointer(window));
        w->OnKeyEvent(key, action, mods);
    });
}

Window::~Window()
{
    glfwDestroyWindow(mWindow);
    glfwTerminate();
}

void Window::Run()
{
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForOpenGL(mWindow, true);
    ImGui_ImplOpenGL3_Init(nullptr);
    ImGui::StyleColorsDark();

    bool keepAlive = OnCreate();
    FrameDuration elapsedTime(1.0f / 60.0f);

    while (!glfwWindowShouldClose(mWindow) && keepAlive)
    {
        std::chrono::steady_clock::time_point t0 = std::chrono::steady_clock::now();
        mRenderer.Clear();

        keepAlive = OnUpdate(elapsedTime);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        OnImGuiUpdate();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
        std::chrono::steady_clock::time_point t1 = std::chrono::steady_clock::now();
        elapsedTime = std::chrono::duration_cast<FrameDuration>(t1 - t0);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool Window::OnCreate()
{
    std::cerr << "Override Window::OnCreate method\n";
    return false;
}

bool Window::OnUpdate(FrameDuration elapsedTime)
{
    (void)elapsedTime;
    std::cerr << "Override Window::OnUpdate method\n";
    return false;
}

void Window::OnImGuiUpdate()
{

}

void Window::OnKeyEvent(int key, int action, int mods)
{
    (void)key;
    (void)action;
    (void)mods;
}
