#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string_view>
#include <memory>
#include <fstream>
#include <iterator>
#include <algorithm>

struct ShaderSources {
    std::string VertexShader;
    std::string FragmentShader;
    bool LoadStatus;
};

static ShaderSources LoadShaders(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
{
    std::ifstream istream;
    ShaderSources result{ "", "", false };

    istream.open(vertexShaderPath.data(), std::ifstream::in);
    if (!istream) {
        std::cerr << "LoadShaders() for path " << vertexShaderPath << " failed\n";
        return result;
    }

    std::copy(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>(), std::back_inserter(result.VertexShader));
    istream.close();

    istream.open(fragmentShaderPath.data(), std::ifstream::in);
    if (!istream) {
        std::cerr << "LoadShaders() for path " << fragmentShaderPath << " failed\n";
        return result;
    }

    std::copy(std::istreambuf_iterator<char>(istream), std::istreambuf_iterator<char>(), std::back_inserter(result.FragmentShader));
    istream.close();

    result.LoadStatus = true;
    return result;
}

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
    window = glfwCreateWindow(640, 480, "Ray Marching", NULL, NULL);
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
        return -1;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float vertices[] = {
        -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f,  1.0f, 1.0f, 1.0f, 0.0f,
    };

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof vertices, &vertices, GL_STATIC_DRAW);

    unsigned int index_buffer;
    glGenBuffers(1, &index_buffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, &indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    ShaderSources shaders = LoadShaders("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
    if (!shaders.LoadStatus) {
        return -1;
    }
    unsigned int shader = CreateShader(shaders.VertexShader, shaders.FragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return(0);
}