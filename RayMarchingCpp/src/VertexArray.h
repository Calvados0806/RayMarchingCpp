#pragma once

namespace OpenGL {

    class VertexBuffer;
    class VertexLayout;

    class VertexArray {
    public:
        VertexArray();
        
        void Bind() const;
        void Unbind() const;
        void Delete() const;
        void LinkLayout(const VertexBuffer& buffer, const VertexLayout& layout) const;
    private:
        unsigned int mOpenGLID;
    };

}