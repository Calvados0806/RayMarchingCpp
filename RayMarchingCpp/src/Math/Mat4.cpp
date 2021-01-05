#include "Mat4.h"
#include "../OpenGL/GLCore.h"

#include <algorithm>

using namespace Math;

Mat4::Mat4() : Mat4(0.0f)
{
}

Mat4::Mat4(float value) : Mat4({
        value, 0.0f, 0.0f, 0.0f,
        0.0f, value , 0.0f, 0.0f,
        0.0f, 0.0f, value, 0.0f,
        0.0f, 0.0f, 0.0f, value,
    })
{
}
Mat4::Mat4(std::initializer_list<float> list)
{
    ASSERT(list.size() >= 16);
    std::copy_n(list.begin(), 16, data());
}

Mat4::Mat4(std::initializer_list<Vec4> list)
{
    ASSERT(list.size() >= 4);
    for (unsigned int i = 0; i < 4; i++) {
        std::copy_n((list.begin() + i)->data(), 4, mData[i].data());
    }
}

const float* Math::Mat4::data() const
{
    return mData[0].data();
}

float* Math::Mat4::data()
{
    return mData[0].data();
}

unsigned int Math::Mat4::size() const
{
    return 16;
}

const Vec4& Mat4::operator[](unsigned int i) const
{
    ASSERT(i < 4);
    return mData[i];
}

Vec4& Mat4::operator[](unsigned int i)
{
    ASSERT(i < 4);
    return mData[i];
}

Mat4 Math::Mat4::operator-() const
{
    return Mat4({
        -mData[0],
        -mData[1],
        -mData[2],
        -mData[3]
    });
}

Mat4 Mat4::operator+(const Mat4& other) const
{
    return Mat4({
        mData[0] + other[0],
        mData[1] + other[1],
        mData[2] + other[2],
        mData[3] + other[3]
    });
}

Mat4 Mat4::operator-(const Mat4& other) const
{
    return Mat4({
        mData[0] - other[0],
        mData[1] - other[1],
        mData[2] - other[2],
        mData[3] - other[3]
    });
}

Vec4 Mat4::operator*(const Vec4& vec) const
{
    return this->Dot(vec);
}

Mat4 Mat4::operator*(const Mat4& other) const
{
    return this->Dot(other);
}

Mat4 Mat4::Transpose() const
{
    return Mat4({
        mData[0][0], mData[1][0], mData[2][0], mData[3][0],
        mData[0][1], mData[1][1], mData[2][1], mData[3][1],
        mData[0][2], mData[1][2], mData[2][2], mData[3][2],
        mData[0][3], mData[1][3], mData[2][3], mData[3][3]
    });
}

Mat4 Mat4::Dot(const Mat4& mat) const
{
    return Mat4({
        mData[0].Dot(mat),
        mData[1].Dot(mat),
        mData[2].Dot(mat),
        mData[3].Dot(mat)
    });
}

Vec4 Math::Mat4::Dot(const Vec4& vec) const
{
    return Vec4(
        vec[0] * mData[0][0] + vec[1] * mData[1][0] + vec[2] * mData[2][0] + vec[3] * mData[3][0],
        vec[0] * mData[0][1] + vec[1] * mData[1][1] + vec[2] * mData[2][1] + vec[3] * mData[3][1],
        vec[0] * mData[0][2] + vec[1] * mData[1][2] + vec[2] * mData[2][2] + vec[3] * mData[3][2],
        vec[0] * mData[0][3] + vec[1] * mData[1][3] + vec[2] * mData[2][3] + vec[3] * mData[3][3]
    );
}

Mat4 Math::Mat4::CreateTranslation(float dx, float dy, float dz)
{
    return Mat4({
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        dx,   dy,   dz,   1.0f
    });
}

Mat4 Math::Mat4::CreateRotationX(float angle)
{
    return Mat4({
        1.0f,  0.0f,            0.0f,            0.0f,
        0.0f,  std::cos(angle), std::sin(angle), 0.0f,
        0.0f, -std::sin(angle), std::cos(angle), 0.0f,
        0.0f,  0.0f,            0.0f,            1.0f
    });
}

Mat4 Math::Mat4::CreateRotationY(float angle)
{
    return Mat4({
        std::cos(angle), 0.0f, -std::sin(angle), 0.0f,
        0.0f,            1.0f,  0.0f,            0.0f,
        std::sin(angle), 0.0f,  std::cos(angle), 0.0f,
        0.0f,            0.0f,  0.0f,            1.0f
    });
}

Mat4 Math::Mat4::CreateRotationZ(float angle)
{
    return Mat4({
         std::cos(angle), std::sin(angle), 0.0f, 0.0f,
        -std::sin(angle), std::cos(angle), 0.0f, 0.0f,
         0.0f,            0.0f,            1.0f, 0.0f,
         0.0f,            0.0f,            0.0f, 1.0f
    });
}

Mat4 Math::Mat4::CreateScale(float sx, float sy, float sz)
{
    return Mat4({
        sx,   0.0f, 0.0f, 0.0f,
        0.0f, sy,   0.0f, 0.0f,
        0.0f, 0.0f, sz,   0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    });
}

Mat4 Mat4::operator/(const Mat4& other) const
{
    return Mat4({
        mData[0] / other[0],
        mData[1] / other[1],
        mData[2] / other[2],
        mData[3] / other[3]
    });
}

Mat4 Mat4::operator+(float value) const
{
    return Mat4({
        mData[0] + value,
        mData[1] + value,
        mData[2] + value,
        mData[3] + value
    });
}

Mat4 Mat4::operator-(float value) const
{
    return Mat4({
        mData[0] - value,
        mData[1] - value,
        mData[2] - value,
        mData[3] - value
    });
}

Mat4 Mat4::operator*(float value) const
{
    return Mat4({
        mData[0] * value,
        mData[1] * value,
        mData[2] * value,
        mData[3] * value
    });
}

Mat4 Mat4::operator/(float value) const
{
    return Mat4({
        mData[0] / value,
        mData[1] / value,
        mData[2] / value,
        mData[3] / value
    });
}

Mat4 Math::operator+(float value, const Mat4& mat)
{
    return mat + value;
}

Mat4 Math::operator-(float value, const Mat4& mat)
{
    return -mat + value;
}

Mat4 Math::operator*(float value, const Mat4& mat)
{
    return mat * value;
}

Mat4 Math::operator/(float value, const Mat4& mat)
{
    return Mat4({
        value / mat[0],
        value / mat[1],
        value / mat[2],
        value / mat[3]
    });
}

std::ostream& Math::operator<<(std::ostream& out, const Mat4& mat)
{
    out << "(\n\t" << mat[0] << "\n\t" << mat[1] << "\n\t" << mat[2] << "\n\t" << mat[3] << "\n)";
    return out;
}
