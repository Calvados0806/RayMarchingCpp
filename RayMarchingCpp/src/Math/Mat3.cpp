#include "Mat3.h"
#include "../OpenGL/GLCore.h"

#include <algorithm>

using namespace Math;

Mat3::Mat3() : Mat3(0.0f)
{
}

Mat3::Mat3(float value) : Mat3({
        value, 0.0f, 0.0f,
        0.0f, value , 0.0f,
        0.0f, 0.0f, value
    })
{
}
Mat3::Mat3(std::initializer_list<float> list)
{
    ASSERT(list.size() >= 9);
    std::copy_n(list.begin(), 9, data());
}

Mat3::Mat3(std::initializer_list<Vec3> list)
{
    ASSERT(list.size() >= 3);
    for (unsigned int i = 0; i < 3; i++) {
        std::copy_n((list.begin() + i)->data(), 3, mData[i].data());
    }
}

const float* Math::Mat3::data() const
{
    return mData[0].data();
}

float* Math::Mat3::data()
{
    return mData[0].data();
}

unsigned int Math::Mat3::size() const
{
    return 9;
}

const Vec3& Mat3::operator[](unsigned int i) const
{
    ASSERT(i < 3);
    return mData[i];
}

Vec3& Mat3::operator[](unsigned int i)
{
    ASSERT(i < 3);
    return mData[i];
}

Mat3 Math::Mat3::operator-() const
{
    return Mat3({
        -mData[0],
        -mData[1],
        -mData[2]
    });
}

Mat3 Mat3::operator+(const Mat3& other) const
{
    return Mat3({
        mData[0] + other[0],
        mData[1] + other[1],
        mData[2] + other[2]
    });
}

Mat3 Mat3::operator-(const Mat3& other) const
{
    return Mat3({
        mData[0] - other[0],
        mData[1] - other[1],
        mData[2] - other[2]
    });
}

Vec3 Mat3::operator*(const Vec3& vec) const
{
    return this->Dot(vec);
}

Mat3 Mat3::operator*(const Mat3& other) const
{
    return this->Dot(other);
}

Mat3 Mat3::Transpose() const
{
    return Mat3({
        mData[0][0], mData[1][0], mData[2][0],
        mData[0][1], mData[1][1], mData[2][1],
        mData[0][2], mData[1][2], mData[2][2]
    });
}

Mat3 Mat3::Dot(const Mat3& mat) const
{
    return Mat3({
        mData[0].Dot(mat),
        mData[1].Dot(mat),
        mData[2].Dot(mat)
    });
}

Vec3 Math::Mat3::Dot(const Vec3& vec) const
{
    return Vec3(
        vec[0] * mData[0][0] + vec[1] * mData[1][0] + vec[2] * mData[2][0],
        vec[0] * mData[0][1] + vec[1] * mData[1][1] + vec[2] * mData[2][1],
        vec[0] * mData[0][2] + vec[1] * mData[1][2] + vec[2] * mData[2][2]
    );
}

Mat3 Mat3::operator/(const Mat3& other) const
{
    return Mat3({
        mData[0] / other[0],
        mData[1] / other[1],
        mData[2] / other[2]
    });
}

Mat3 Mat3::operator+(float value) const
{
    return Mat3({
        mData[0] + value,
        mData[1] + value,
        mData[2] + value
    });
}

Mat3 Mat3::operator-(float value) const
{
    return Mat3({
        mData[0] - value,
        mData[1] - value,
        mData[2] - value
    });
}

Mat3 Mat3::operator*(float value) const
{
    return Mat3({
        mData[0] * value,
        mData[1] * value,
        mData[2] * value
    });
}

Mat3 Mat3::operator/(float value) const
{
    return Mat3({
        mData[0] / value,
        mData[1] / value,
        mData[2] / value
    });
}

Mat3 Math::operator+(float value, const Mat3& mat)
{
    return mat + value;
}

Mat3 Math::operator-(float value, const Mat3& mat)
{
    return -mat + value;
}

Mat3 Math::operator*(float value, const Mat3& mat)
{
    return mat * value;
}

Mat3 Math::operator/(float value, const Mat3& mat)
{
    return Mat3({
        value / mat[0],
        value / mat[1],
        value / mat[2]
    });
}

std::ostream& Math::operator<<(std::ostream& out, const Mat3& mat)
{
    out << "(\n\t" << mat[0] << "\n\t" << mat[1] << "\n\t" << mat[2] << "\n)";
    return out;
}