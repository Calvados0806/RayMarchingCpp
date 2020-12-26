#include "IndexBuffer.h"
#include "GLCore.h"

using namespace OpenGL;

IndexBuffer::IndexBuffer(const uint32_t* data, size_t count) : mCount(count)
{
    static_assert(sizeof(uint32_t) == sizeof(GLuint));
    GLCall(glGenBuffers(1, &mOpenGLID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mOpenGLID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mOpenGLID));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

size_t IndexBuffer::Count() const
{
    return mCount;
}

void IndexBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &mOpenGLID));
}
