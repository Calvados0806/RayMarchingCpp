#include "Vec2.h"
#include "Mat2.h"
#include "../OpenGL/GLCore.h"

#include <cmath>
#include <algorithm>

using namespace Math;

Vec2::Vec2() : mData{ 0.0f, 0.0f }
{
}

Vec2::Vec2(std::initializer_list<float> list)
{
    ASSERT(list.size() >= 2);
    std::copy_n(list.begin(), 2, mData);
}

Vec2::Vec2(float value) : mData{ value, value }
{
}

Vec2::Vec2(float v1, float v2) : mData{ v1, v2 }
{
}

Vec2::Vec2(float arr[2]) : mData{ arr[0], arr[1] }
{
}

float& Vec2::x()
{
    return mData[0];
}

float Vec2::x() const
{
    return mData[0];
}

float& Vec2::y()
{
    return mData[1];
}

float Vec2::y() const
{
    return mData[1];
}

const float* Math::Vec2::data() const
{
    return mData;
}

float* Math::Vec2::data()
{
    return mData;
}

unsigned int Math::Vec2::size() const
{
    return 2;
}

float Vec2::operator[](unsigned int i) const
{
    ASSERT(i < 2);
    return mData[i];
}

float& Vec2::operator[](unsigned int i)
{
    ASSERT(i < 2);
    return mData[i];
}

Vec2 Vec2::operator-() const
{
    return Vec2(-x(), -y());
}

Vec2 Vec2::operator+(const Vec2& other) const
{
    return Vec2(x() + other.x(), y() + other.y());
}

Vec2 Vec2::operator-(const Vec2& other) const
{
    return Vec2(x() - other.x(), y() - other.y());
}

Vec2 Vec2::operator*(const Vec2& other) const
{
    return Vec2(x() * other.x(), y() * other.y());
}

Vec2 Vec2::operator/(const Vec2& other) const
{
    return Vec2(x() / other.x(), y() / other.y());
}

Vec2 Vec2::operator+(float value) const
{
    return Vec2(x() + value, y() + value);
}

Vec2 Vec2::operator-(float value) const
{
    return Vec2(x() - value, y() - value);
}

Vec2 Vec2::operator*(float value) const
{
    return Vec2(x() * value, y() * value);
}

Vec2 Vec2::operator/(float value) const
{
    return Vec2(x() / value, y() / value);
}

float Vec2::Dot(const Vec2& other) const
{
    return x() * other.x() + y() * other.y();
}

Vec2 Math::Vec2::Dot(const Mat2& mat) const
{
    return mat.Dot(*this);
}

Vec2 Vec2::Normalize() const
{
    return (*this) / Magnitude();
}

float Vec2::Magnitude() const
{
    return std::sqrt(MagnitudeSquared());
}

float Vec2::MagnitudeSquared() const
{
    return x() * x() + y() * y();
}

Vec2 Vec2::Translate(const Vec2& shift) const
{
    return (*this) + shift;
}

Vec2 Vec2::Rotate(float angle) const
{
    return this->Dot(Mat2({
         std::cos(angle), std::sin(angle),
        -std::sin(angle), std::cos(angle)
    }));
}

Vec2 Vec2::Scale(float value) const
{
    return (*this) * value;
}

Vec2 Math::operator+(float value, const Vec2& vec)
{
    return vec + value;
}

Vec2 Math::operator-(float value, const Vec2& vec)
{
    return -vec + value;
}

Vec2 Math::operator*(float value, const Vec2& vec)
{
    return vec * value;
}

Vec2 Math::operator/(float value, const Vec2& vec)
{
    return Vec2(value / vec.x(), value / vec.y());
}

std::ostream& Math::operator<<(std::ostream& out, const Vec2& vec)
{
    out << "(" << vec[0] << ", " << vec[1] << ")";
    return out;
}