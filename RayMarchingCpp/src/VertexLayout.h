#pragma once

#include "GLCore.h"

#include <vector>

namespace OpenGL {

    class VertexLayout {
    public:
        struct Attribute {
            unsigned int  Count;
            unsigned int  GLType;
            unsigned char Normalized;
            unsigned int  Offset;
        };
    public:
        template <typename T>
        void AddAttribute(unsigned int count) = delete;

        template <>
        void AddAttribute<float>(unsigned int count)
        {
            static_assert(sizeof(float) == sizeof(GLfloat));
            mAttributes.push_back({ count, GL_FLOAT, GL_FALSE, mStride });
            mStride += sizeof(float) * count;
        }

        template <>
        void AddAttribute<unsigned int>(unsigned int count)
        {
            static_assert(sizeof(unsigned int) == sizeof(GLuint));
            mAttributes.push_back({ count, GL_UNSIGNED_INT, GL_FALSE, mStride });
            mStride += sizeof(unsigned int) * count;
        }

        template <>
        void AddAttribute<unsigned char>(unsigned int count)
        {
            static_assert(sizeof(unsigned char) == sizeof(GLubyte));
            mAttributes.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE, mStride });
            mStride += sizeof(unsigned char) * count;
        }

        unsigned int Stride() const { return mStride; }

        const std::vector<Attribute>& Attributes() const { return mAttributes; }
    private:
        std::vector<Attribute> mAttributes;
        unsigned int mStride = 0;
    };

}