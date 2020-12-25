#include "GLCore.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexLayout.h"
#include "ShaderProgram.h"

#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>
#include <memory>
#include <fstream>
#include <iterator>
#include <algorithm>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "glfwInit() failed\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const float window_width = 640.0f;
    const float window_height = 480.0f;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(window_width, window_height, "Ray Marching", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cerr << "glfwCreateWindow() failed\n";
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(0);

    if (glewInit() != GLEW_OK) {
        std::cerr << "glewInit() failed\n";
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float vertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         1.0f,  1.0f,
        -1.0f,  1.0f
    };
    uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

    OpenGL::VertexArray vao;
    OpenGL::VertexBuffer vbo(vertices, sizeof vertices);
    
    OpenGL::VertexLayout layout;
    layout.AddAttribute<float>(2);
    vao.LinkLayout(vbo, layout);

    OpenGL::IndexBuffer ibo(indices, std::size(indices));

    OpenGL::ShaderProgram shader("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
    shader.Bind();
    shader.SetUniform2f("u_Resolution", window_width, window_height);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glDrawElements(GL_TRIANGLES, ibo.Count(), GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    shader.Delete();
    ibo.Delete();
    vbo.Delete();
    vao.Delete();

    glfwTerminate();
    return(0);
}