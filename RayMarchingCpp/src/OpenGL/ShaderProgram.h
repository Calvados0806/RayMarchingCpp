#pragma once

#include <string_view>
#include <unordered_map>
#include <memory>

#include "ShaderSource.h"

namespace OpenGL {

    class ShaderProgram {
    public:
        ShaderProgram() = default;
        ShaderProgram(const std::string_view vertexShaderSrc, const std::string_view fragmentShaderSrc);

        void Bind() const;
        void Unbind() const;
        void Delete() const;

        void SetUniform1i(const std::string_view name, int v);
        void SetUniform1f(const std::string_view name, float v1);
        void SetUniform2f(const std::string_view name, float v1, float v2);
        void SetUniform3f(const std::string_view name, float v1, float v2, float v3);
        void SetUniform4f(const std::string_view name, float v1, float v2, float v3, float v4);
        void SetUniformBool(const std::string_view name, bool v);

        static std::shared_ptr<ShaderProgram> LoadFromFiles(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath);
        static std::shared_ptr<ShaderProgram> FromSources(std::shared_ptr<ShaderSource> vertexShader, std::shared_ptr<ShaderSource> fragmentShader);
    private:
        struct ShaderSources {
            std::string VertexShader;
            std::string FragmentShader;
            bool LoadStatus;
        };

        int GetUniformLocation(const std::string_view name) const;
        static ShaderSources LoadShaders(const std::string_view vertexShaderPath, const std::string_view fragmentShaderPath);
        static unsigned int CompileShader(unsigned int type, const std::string_view source);
        static unsigned int CreateShader(const std::string_view vertexShader, const std::string_view fragmentShader);
    private:
        unsigned int mOpenGLID;
        mutable std::unordered_map<std::string, int> mUniformCache;
    };

}