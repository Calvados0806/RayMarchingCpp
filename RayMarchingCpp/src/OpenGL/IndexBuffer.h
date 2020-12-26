#pragma once

#include <cstddef>
#include <cinttypes>

namespace OpenGL {

    class IndexBuffer {
    public:
        IndexBuffer() = default;
        IndexBuffer(const uint32_t* data, size_t count);

        void Bind() const;
        void Unbind() const;
        size_t Count() const;
        void Delete() const;
    private:
        unsigned int mOpenGLID;
        size_t mCount;
    };

}