#include "Vec4.h"
#include "Mat4.h"
#include "../OpenGL/GLCore.h"

#include <cmath>
#include <algorithm>

using namespace Math;

Vec4::Vec4() : mData{ 0.0f, 0.0f, 0.0f, 0.0f }
{
}

Vec4::Vec4(float value) : mData{ value, value, value, value }
{
}

Vec4::Vec4(std::initializer_list<float> list)
{
    ASSERT(list.size() >= 4);
    std::copy_n(list.begin(), 4, mData);
}

Vec4::Vec4(float v1, float v2, float v3, float v4) : mData{ v1, v2, v3, v4 }
{
}

Vec4::Vec4(float arr[4]) : mData{ arr[0], arr[1], arr[2], arr[3] }
{
}

Vec4::Vec4(const Vec3& vec3) : Vec4(vec3, 0.0f)
{
}

Vec4::Vec4(const Vec3& vec3, float v4) : mData{ vec3.x(), vec3.y(), vec3.z(), v4 }
{
}

Vec4::Vec4(const Vec2& vec1, const Vec2& vec2) : mData{ vec1.x(), vec1.y(), vec2.x(), vec2.y() }
{
}

Vec4::Vec4(const Vec2& vec2, float v3, float v4) : mData{ vec2.x(), vec2.y(), v3, v4 }
{
}

Vec4::Vec4(const Vec2& vec2) : Vec4(vec2, 0.0f, 0.0f)
{
}

Vec4::operator Vec3() const
{
    return Vec3(x(), y(), z());
}

Vec4::operator Vec2() const
{
    return Vec2(x(), y());
}

float& Vec4::x()
{
    return mData[0];
}

float Vec4::x() const
{
    return mData[0];
}

float& Vec4::y()
{
    return mData[1];
}

float Vec4::y() const
{
    return mData[1];
}

float& Vec4::z()
{
    return mData[2];
}

float Vec4::z() const
{
    return mData[2];
}

float& Vec4::w()
{
    return mData[3];
}

float Vec4::w() const
{
    return mData[3];
}

float& Vec4::r()
{
    return mData[0];
}

float Vec4::r() const
{
    return mData[0];
}

float& Vec4::g()
{
    return mData[1];
}

float Vec4::g() const
{
    return mData[1];
}

float& Vec4::b()
{
    return mData[2];
}

float Vec4::b() const
{
    return mData[2];
}

float& Vec4::a()
{
    return mData[3];
}

float Vec4::a() const
{
    return mData[3];
}

const float* Math::Vec4::data() const
{
    return mData;
}

float* Math::Vec4::data()
{
    return mData;
}

unsigned int Math::Vec4::size() const
{
    return 4;
}

float Vec4::operator[](unsigned int i) const
{
    ASSERT(i < 4);
    return mData[i];
}

float& Vec4::operator[](unsigned int i)
{
    ASSERT(i < 4);
    return mData[i];
}

Vec4 Vec4::operator-() const
{
    return Vec4(-x(), -y(), -z(), -w());
}

Vec4 Vec4::operator+(const Vec4& other) const
{
    return Vec4(x() + other.x(), y() + other.y(), z() + other.z(), w() + other.w());
}

Vec4 Vec4::operator-(const Vec4& other) const
{
    return Vec4(x() - other.x(), y() - other.y(), z() - other.z(), w() - other.w());
}

Vec4 Vec4::operator*(const Vec4& other) const
{
    return Vec4(x() * other.x(), y() * other.y(), z() * other.z(), w() * other.w());
}

Vec4 Vec4::operator/(const Vec4& other) const
{
    return Vec4(x() / other.x(), y() / other.y(), z() / other.z(), w() / other.w());
}

Vec4 Vec4::operator+(float value) const
{
    return Vec4(x() + value, y() + value, z() + value, w() + value);
}

Vec4 Vec4::operator-(float value) const
{
    return Vec4(x() - value, y() - value, z() - value, w() - value);
}

Vec4 Vec4::operator*(float value) const
{
    return Vec4(x() * value, y() * value, z() * value, w() * value);
}

Vec4 Vec4::operator/(float value) const
{
    return Vec4(x() / value, y() / value, z() / value, w() / value);
}

Vec4 Vec4::operator*(const Mat4& mat) const
{
    return this->Dot(mat);
}

float Vec4::Dot(const Vec4& other) const
{
    return x()*other.x() + y()*other.y() + z()*other.z() + w()*other.w();
}

Vec4 Math::Vec4::Dot(const Mat4& mat) const
{
    return mat.Dot(*this);
}

Vec4 Vec4::Normalize() const
{
    return (*this) / Magnitude();
}

float Vec4::Magnitude() const
{
    return std::sqrt(MagnitudeSquared());
}

float Vec4::MagnitudeSquared() const
{
    return x() * x() + y() * y() + z() * z() + w() * w();
}

Vec4 Vec4::Translate(const Vec4& shift) const
{
    return (*this) + shift;
}

Vec4 Vec4::Scale(float value) const
{
    return (*this) * value;
}

Vec4 Math::operator+(float value, const Vec4& vec)
{
    return vec + value;
}

Vec4 Math::operator-(float value, const Vec4& vec)
{
    return -vec + value;
}

Vec4 Math::operator*(float value, const Vec4& vec)
{
    return vec * value;
}

Vec4 Math::operator/(float value, const Vec4& vec)
{
    return Vec4(value / vec.x(), value / vec.y(), value / vec.z(), value / vec.w());
}

std::ostream& Math::operator<<(std::ostream& out, const Vec4& vec)
{
    out << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ", " << vec[3] << ")";
    return out;
}
