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

static void GLClearError()
{
    while (glGetError() != GL_NO_ERROR)
        ;
}

static bool GLCheckForErrors()
{
    bool no_errors_occurred = true;
    while (GLenum error = glGetError()) {
        std::cerr << "OpenGL Error occurred: 0x" << std::hex << error << std::endl;
        no_errors_occurred = false;
    }
    return no_errors_occurred;
}


#ifdef _DEBUG

#define ASSERT(x) if (!(x)) __debugbreak()

#define GLCall(funccall) \
    GLClearError(); \
    funccall; \
    ASSERT(GLCheckForErrors())

#else

#define ASSERT(x) ((void)0)

#define GLCall(funccall) funccall

#endif // _DEBUG


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
    GLCall(unsigned int shader_id = glCreateShader(type));
    const char* src = source.data();
    GLCall(glShaderSource(shader_id, 1, &src, nullptr));
    GLCall(glCompileShader(shader_id));

    int result;
    GLCall(glGetShaderiv(shader_id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLCall(glGetShaderiv(shader_id, GL_INFO_LOG_LENGTH, &length));
        std::unique_ptr<char[]> message = std::make_unique<char[]>(length);
        GLCall(glGetShaderInfoLog(shader_id, length, &length, message.get()));
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << 
            " shader: " << message.get() << std::endl;
        GLCall(glDeleteShader(shader_id));
        return 0;
    }

    return shader_id;
}

static unsigned int CreateShader(const std::string_view vertexShader, const std::string_view fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER,fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

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

    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

    unsigned int vertex_buffer;
    GLCall(glGenBuffers(1, &vertex_buffer));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer));
    GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof vertices, &vertices, GL_STATIC_DRAW));

    unsigned int index_buffer;
    GLCall(glGenBuffers(1, &index_buffer));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, &indices, GL_STATIC_DRAW));

    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0));
    GLCall(glEnableVertexAttribArray(0));

    ShaderSources shaders = LoadShaders("res/shaders/Vertex.shader", "res/shaders/Fragment.shader");
    if (!shaders.LoadStatus) {
        return -1;
    }
    unsigned int shader = CreateShader(shaders.VertexShader, shaders.FragmentShader);
    GLCall(glUseProgram(shader));

    GLCall(int resolution_location = glGetUniformLocation(shader, "u_Resolution"));
    ASSERT(resolution_location != -1);
    GLCall(glUniform2f(resolution_location, window_width, window_height));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        GLCall(glDrawElements(GL_TRIANGLES, std::size(indices), GL_UNSIGNED_INT, nullptr));

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    GLCall(glDeleteProgram(shader));

    glfwTerminate();
    return(0);
}