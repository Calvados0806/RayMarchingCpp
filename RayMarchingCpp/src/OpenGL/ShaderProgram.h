#pragma once

#include <string_view>

namespace OpenGL {

    class ShaderProgram {
    public:
        ShaderProgram(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath);

        void Bind() const;
        void Unbind() const;
        void Delete() const;

        void SetUniform2f(const std::string_view name, float v1, float v2);
    private:
        struct ShaderSources {
            std::string VertexShader;
            std::string FragmentShader;
            bool LoadStatus;
        };

        int GetUniformLocation(const std::string_view name);
        static ShaderSources LoadShaders(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath);
        static unsigned int CompileShader(unsigned int type, const std::string_view source);
        static unsigned int CreateShader(const std::string_view vertexShader, const std::string_view fragmentShader);
    private:
        unsigned int mOpenGLID;
    };

}