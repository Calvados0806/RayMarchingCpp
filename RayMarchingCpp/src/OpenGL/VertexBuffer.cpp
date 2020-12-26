#include "VertexBuffer.h"
#include "GLCore.h"

using namespace OpenGL;

VertexBuffer::VertexBuffer(const void* data, size_t size)
{
    GLCall(glGenBuffers(1, &mOpenGLID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mOpenGLID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, mOpenGLID));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void VertexBuffer::Delete() const
{
    GLCall(glDeleteBuffers(1, &mOpenGLID));
}
