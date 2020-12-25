#include "VertexArray.h"
#include "GLCore.h"
#include "VertexBuffer.h"
#include "VertexLayout.h"

using namespace OpenGL;

VertexArray::VertexArray()
{
    GLCall(glGenVertexArrays(1, &mOpenGLID));
}

void VertexArray::Bind() const
{
    GLCall(glBindVertexArray(mOpenGLID));
}

void VertexArray::Unbind() const
{
    GLCall(glBindVertexArray(0));
}

void VertexArray::Delete() const
{
    GLCall(glDeleteVertexArrays(1, &mOpenGLID));
}

void VertexArray::LinkLayout(const VertexBuffer& buffer, const VertexLayout& layout) const
{
    Bind();
    buffer.Bind();
    const std::vector<VertexLayout::Attribute>& attributes = layout.Attributes();

    for (unsigned int index = 0; index < attributes.size(); index++) {
        const VertexLayout::Attribute& attribute = attributes[index];
        GLCall(glEnableVertexAttribArray(index));
        GLCall(glVertexAttribPointer(index, attribute.Count, attribute.GLType, 
            attribute.Normalized, layout.Stride(), reinterpret_cast<const void*>(attribute.Offset)));
    }
}
