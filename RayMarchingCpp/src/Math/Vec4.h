#pragma once

#include <initializer_list>
#include <iostream>

#include "Vec3.h"

namespace Math {
    class Mat4;

    class Vec4 {
    public:
        Vec4();
        explicit Vec4(float value);
        Vec4(std::initializer_list<float> list);
        Vec4(float v1, float v2, float v3, float v4);
        Vec4(float arr[4]);
        Vec4(const Vec3& vec3);
        Vec4(const Vec3& vec3, float v4);
        Vec4(const Vec2& vec1, const Vec2& vec2);
        Vec4(const Vec2& vec2, float v3, float v4);
        Vec4(const Vec2& vec2);

        explicit operator Vec3() const;
        explicit operator Vec2() const;

        float& x();
        float  x() const;
        float& y();
        float  y() const;
        float& z();
        float  z() const;
        float& w();
        float  w() const;

        float& r();
        float  r() const;
        float& g();
        float  g() const;
        float& b();
        float  b() const;
        float& a();
        float  a() const;

        const float* data() const;
        float* data();
        unsigned int size() const;

        float operator[](unsigned int i) const;
        float& operator[](unsigned int i);

        Vec4 operator-() const;
        Vec4 operator+(const Vec4& other) const;
        Vec4 operator-(const Vec4& other) const;
        /*
            Component-wise multiplication
        */
        Vec4 operator*(const Vec4& other) const;
        /*
            Component-wise division
        */
        Vec4 operator/(const Vec4& other) const;

        Vec4 operator+(float value) const;
        Vec4 operator-(float value) const;
        Vec4 operator*(float value) const;
        Vec4 operator/(float value) const;

        Vec4 operator*(const Mat4& mat) const;

        float Dot(const Vec4& other) const;
        Vec4 Dot(const Mat4& mat) const;
        Vec4 Normalize() const;
        float Magnitude() const;
        /*
            Same as .Magnitude(), but without calculating sqrt()
        */
        float MagnitudeSquared() const;
        Vec4 Translate(const Vec4& shift) const;
        Vec4 Scale(float value) const;
    private:
        float mData[4];
    };

    Vec4 operator+(float value, const Vec4& vec);
    Vec4 operator-(float value, const Vec4& vec);
    Vec4 operator*(float value, const Vec4& vec);
    Vec4 operator/(float value, const Vec4& vec);

    std::ostream& operator<<(std::ostream& out, const Vec4& vec);
}