#pragma once

#include <cstddef>

namespace OpenGL {

    class VertexBuffer {
    public:
        VertexBuffer(const void* data, size_t size);

        void Bind() const;
        void Unbind() const;
        void Delete() const;
    private:
        unsigned int mOpenGLID;
    };

}