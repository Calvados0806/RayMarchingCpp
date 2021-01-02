#include "Mat2.h"
#include "../OpenGL/GLCore.h"

#include <algorithm>

using namespace Math;

Mat2::Mat2() : Mat2(0.0f)
{
}

Mat2::Mat2(float value) : Mat2({
        value, 0.0f,
        0.0f, value
    })
{
}
Mat2::Mat2(std::initializer_list<float> list)
{
    ASSERT(list.size() >= 4);
    std::copy_n(list.begin(), 4, data());
}

Mat2::Mat2(std::initializer_list<Vec2> list)
{
    ASSERT(list.size() >= 2);
    for (unsigned int i = 0; i < 2; i++) {
        std::copy_n((list.begin() + i)->data(), 2, mData[i].data());
    }
}

const float* Math::Mat2::data() const
{
    return mData[0].data();
}

float* Math::Mat2::data()
{
    return mData[0].data();
}

unsigned int Math::Mat2::size() const
{
    return 4;
}

const Vec2& Mat2::operator[](unsigned int i) const
{
    ASSERT(i < 2);
    return mData[i];
}

Vec2& Mat2::operator[](unsigned int i)
{
    ASSERT(i < 2);
    return mData[i];
}

Mat2 Math::Mat2::operator-() const
{
    return Mat2({
        -mData[0],
        -mData[1]
    });
}

Mat2 Mat2::operator+(const Mat2& other) const
{
    return Mat2({
        mData[0] + other[0],
        mData[1] + other[1]
    });
}

Mat2 Mat2::operator-(const Mat2& other) const
{
    return Mat2({
        mData[0] - other[0],
        mData[1] - other[1]
    });
}

Vec2 Mat2::operator*(const Vec2& vec) const
{
    return this->Dot(vec);
}

Mat2 Mat2::operator*(const Mat2& other) const
{
    return this->Dot(other);
}

Mat2 Mat2::Transpose() const
{
    return Mat2({
        mData[0][0], mData[1][0],
        mData[0][1], mData[1][1]
    });
}

Mat2 Mat2::Dot(const Mat2& mat) const
{
    return Mat2({
        mData[0].Dot(mat),
        mData[1].Dot(mat)
    });
}

Vec2 Math::Mat2::Dot(const Vec2& vec) const
{
    return Vec2(
        vec[0] * mData[0][0] + vec[1] * mData[1][0],
        vec[0] * mData[0][1] + vec[1] * mData[1][1]
    );
}

Mat2 Mat2::operator/(const Mat2& other) const
{
    return Mat2({
        mData[0] / other[0],
        mData[1] / other[1]
    });
}

Mat2 Mat2::operator+(float value) const
{
    return Mat2({
        mData[0] + value,
        mData[1] + value
    });
}

Mat2 Mat2::operator-(float value) const
{
    return Mat2({
        mData[0] - value,
        mData[1] - value
    });
}

Mat2 Mat2::operator*(float value) const
{
    return Mat2({
        mData[0] * value,
        mData[1] * value
    });
}

Mat2 Mat2::operator/(float value) const
{
    return Mat2({
        mData[0] / value,
        mData[1] / value
    });
}

Mat2 Math::operator+(float value, const Mat2& mat)
{
    return mat + value;
}

Mat2 Math::operator-(float value, const Mat2& mat)
{
    return -mat + value;
}

Mat2 Math::operator*(float value, const Mat2& mat)
{
    return mat * value;
}

Mat2 Math::operator/(float value, const Mat2& mat)
{
    return Mat2({
        value / mat[0],
        value / mat[1]
    });
}

std::ostream& Math::operator<<(std::ostream& out, const Mat2& mat)
{
    out << "(\n\t" << mat[0] << "\n\t" << mat[1] << "\n)";
    return out;
}
