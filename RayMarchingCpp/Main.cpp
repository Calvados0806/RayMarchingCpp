#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>
#include <memory>

static unsigned int CompileShader(unsigned int type, const std::string_view source)
{
    unsigned int shader_id = glCreateShader(type);
    const char* src = source.data();
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);

    int result;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        int length;
        glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length);
        std::unique_ptr<char[]> message = std::make_unique<char[]>(length);
        glGetShaderInfoLog(shader_id, length, &length, message.get());
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << 
            " shader: " << message.get() << std::endl;
        glDeleteShader(shader_id);
        return 0;
    }

    return shader_id;
}

static unsigned int CreateShader(const std::string_view vertexShader, const std::string_view fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cerr << "glfwInit() failed\n";
        return -1;
    }

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        std::cerr << "glfwCreateWindow() failed\n";
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        std::cerr << "glewInit() failed\n";
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float vertices[15] = {
        -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
         0.0f,  0.5f, 0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 0.0f, 1.0f
    };

    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, &vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    std::string_view vertex_shader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 color;\n"
        "out vec4 vertexColor;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "   vertexColor = color;\n"
        "}\n";
    std::string_view fragment_shader =
        "#version 330 core\n"
        "\n"
        "in vec4 vertexColor;\n"
        "out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vertexColor;\n"
        "}\n";
    unsigned int shader = CreateShader(vertex_shader, fragment_shader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
	return(0);
}