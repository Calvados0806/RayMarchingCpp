#include "ShaderProgram.h"
#include "GLCore.h"

#include <stdexcept>
#include <memory>
#include <iostream>
#include <fstream>

using namespace OpenGL;

ShaderProgram::ShaderProgram(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
{
    ShaderSources shaders = LoadShaders(vertexShaderPath, fragmentShaderPath);
    if (!shaders.LoadStatus) {
        throw std::runtime_error("Failed to load shaders");
    }
    mOpenGLID = CreateShader(shaders.VertexShader, shaders.FragmentShader);
}

void ShaderProgram::Bind() const
{
    GLCall(glUseProgram(mOpenGLID));
}

void ShaderProgram::Unbind() const
{
    GLCall(glUseProgram(0));
}

void ShaderProgram::Delete() const
{
    GLCall(glDeleteProgram(mOpenGLID));
}

void ShaderProgram::SetUniform2f(const std::string_view name, float v1, float v2)
{
    int location = GetUniformLocation(name);
    GLCall(glUniform2f(location, v1, v2));
}

int ShaderProgram::GetUniformLocation(const std::string_view name)
{
    GLCall(int location = glGetUniformLocation(mOpenGLID, name.data()));
    if (location == -1) {
        std::cout << "[Warning] OpenGL::ShaderProgram::GetUniformLocation for name '" << name << "' failed\n";
    }
    return location;
}

ShaderProgram::ShaderSources ShaderProgram::LoadShaders(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath)
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

unsigned int ShaderProgram::CompileShader(unsigned int type, const std::string_view source)
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

unsigned int ShaderProgram::CreateShader(const std::string_view vertexShader, const std::string_view fragmentShader)
{
    GLCall(unsigned int program = glCreateProgram());
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));

    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));

    return program;
}
