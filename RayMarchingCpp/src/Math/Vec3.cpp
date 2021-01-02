#include "Vec3.h"
#include "Mat3.h"
#include "../OpenGL/GLCore.h"

#include <cmath>
#include <algorithm>

using namespace Math;

Vec3::Vec3() : mData{ 0.0f, 0.0f, 0.0f }
{
}

Vec3::Vec3(float value) : mData{ value, value, value }
{
}

Vec3::Vec3(std::initializer_list<float> list)
{
    ASSERT(list.size() >= 3);
    std::copy_n(list.begin(), 3, mData);
}

Vec3::Vec3(float v1, float v2, float v3) : mData{ v1, v2, v3 }
{
}

Vec3::Vec3(float arr[3]) : mData{ arr[0], arr[1], arr[2] }
{
}

Vec3::Vec3(const Vec2& vec2) : Vec3(vec2, 0.0f)
{
}

Vec3::Vec3(const Vec2& vec2, float v3) : mData{ vec2.x(), vec2.y(), v3 }
{
}

Vec3::operator Vec2() const
{
    return Vec2(x(), y());
}

float& Vec3::x()
{
    return mData[0];
}

float Vec3::x() const
{
    return mData[0];
}

float& Vec3::y()
{
    return mData[1];
}

float Vec3::y() const
{
    return mData[1];
}

float& Vec3::z()
{
    return mData[2];
}

float Vec3::z() const
{
    return mData[2];
}

float& Vec3::r()
{
    return mData[0];
}

float Vec3::r() const
{
    return mData[0];
}

float& Vec3::g()
{
    return mData[1];
}

float Vec3::g() const
{
    return mData[1];
}

float& Vec3::b()
{
    return mData[2];
}

float Vec3::b() const
{
    return mData[2];
}

const float* Math::Vec3::data() const
{
    return mData;
}

float* Math::Vec3::data()
{
    return mData;
}

unsigned int Math::Vec3::size() const
{
    return 3;
}

float Vec3::operator[](unsigned int i) const
{
    ASSERT(i < 3);
    return mData[i];
}

float& Vec3::operator[](unsigned int i)
{
    ASSERT(i < 3);
    return mData[i];
}

Vec3 Vec3::operator-() const
{
    return Vec3(-x(), -y(), -z());
}

Vec3 Vec3::operator+(const Vec3& other) const
{
    return Vec3(x() + other.x(), y() + other.y(), z() + other.z());
}

Vec3 Vec3::operator-(const Vec3& other) const
{
    return Vec3(x() - other.x(), y() - other.y(), z() - other.z());
}

Vec3 Vec3::operator*(const Vec3& other) const
{
    return Vec3(x() * other.x(), y() * other.y(), z() * other.z());
}

Vec3 Vec3::operator/(const Vec3& other) const
{
    return Vec3(x() / other.x(), y() / other.y(), z() / other.z());
}

Vec3 Vec3::operator+(float value) const
{
    return Vec3(x() + value, y() + value, z() + value);
}

Vec3 Vec3::operator-(float value) const
{
    return Vec3(x() - value, y() - value, z() - value);
}

Vec3 Vec3::operator*(float value) const
{
    return Vec3(x() * value, y() * value, z() * value);
}

Vec3 Vec3::operator/(float value) const
{
    return Vec3(x() / value, y() / value, z() / value);
}

float Vec3::Dot(const Vec3& other) const
{
    return x() * other.x() + y() * other.y() + z() * other.z();
}

Vec3 Math::Vec3::Dot(const Mat3& mat) const
{
    return mat.Dot(*this);
}

Vec3 Vec3::Cross(const Vec3& other) const
{
    return Vec3(
            y()*other.z() - z()*other.y(),
            z()*other.x() - x()*other.z(),
            x()*other.y() - y()*other.x()
    );
}

Vec3 Vec3::Normalize() const
{
    return (*this) / Magnitude();
}

float Vec3::Magnitude() const
{
    return std::sqrt(MagnitudeSquared());
}

float Vec3::MagnitudeSquared() const
{
    return x() * x() + y() * y() + z() * z();
}

Vec3 Vec3::Translate(const Vec3& shift) const
{
    return (*this) + shift;
}

Vec3 Vec3::RotateX(float angle) const
{
    return this->Dot(Mat3({
        1.0f,  0.0f,            0.0f,
        0.0f,  std::cos(angle), std::sin(angle),
        0.0f, -std::sin(angle), std::cos(angle)
    }));
}

Vec3 Vec3::RotateY(float angle) const
{
    return this->Dot(Mat3({
        std::cos(angle), 0.0f, -std::sin(angle),
        0.0f,            1.0f, 0.0f,
        std::sin(angle), 0.0f, std::cos(angle)
    }));
}

Vec3 Vec3::RotateZ(float angle) const
{
    return this->Dot(Mat3({
         std::cos(angle), std::sin(angle), 0.0f,
        -std::sin(angle), std::cos(angle), 0.0f,
         0.0f,            0.0f,            1.0f
    }));
}

Vec3 Vec3::Scale(float value) const
{
    return (*this) * value;
}

Vec3 Math::operator+(float value, const Vec3& vec)
{
    return vec + value;
}

Vec3 Math::operator-(float value, const Vec3& vec)
{
    return -vec + value;
}

Vec3 Math::operator*(float value, const Vec3& vec)
{
    return vec * value;
}

Vec3 Math::operator/(float value, const Vec3& vec)
{
    return Vec3(value / vec.x(), value / vec.y(), value / vec.z());
}

std::ostream& Math::operator<<(std::ostream& out, const Vec3& vec)
{
    out << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
    return out;
}
