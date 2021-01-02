#pragma once

#include <initializer_list>
#include <iostream>

#include "Vec2.h"

namespace Math {
    class Mat3;

    class Vec3 {
    public:
        Vec3();
        explicit Vec3(float value);
        Vec3(std::initializer_list<float> list);
        Vec3(float v1, float v2, float v3);
        Vec3(float arr[3]);
        Vec3(const Vec2& vec2);
        Vec3(const Vec2& vec2, float v3);

        explicit operator Vec2() const;

        float& x();
        float  x() const;
        float& y();
        float  y() const;
        float& z();
        float  z() const;

        float& r();
        float  r() const;
        float& g();
        float  g() const;
        float& b();
        float  b() const;

        const float* data() const;
        float* data();
        unsigned int size() const;

        float operator[](unsigned int i) const;
        float& operator[](unsigned int i);

        Vec3 operator-() const;
        Vec3 operator+(const Vec3& other) const;
        Vec3 operator-(const Vec3& other) const;
        /*
            Component-wise multiplication
        */
        Vec3 operator*(const Vec3& other) const;
        /*
            Component-wise division
        */
        Vec3 operator/(const Vec3& other) const;

        Vec3 operator+(float value) const;
        Vec3 operator-(float value) const;
        Vec3 operator*(float value) const;
        Vec3 operator/(float value) const;

        float Dot(const Vec3& other) const;
        Vec3 Dot(const Mat3& mat) const;
        Vec3 Cross(const Vec3& other) const;
        Vec3 Normalize() const;
        float Magnitude() const;
        /*
            Same as .Magnitude(), but without calculating sqrt()
        */
        float MagnitudeSquared() const;
        Vec3 Translate(const Vec3& shift) const;
        Vec3 RotateX(float angle) const;
        Vec3 RotateY(float angle) const;
        Vec3 RotateZ(float angle) const;
        Vec3 Scale(float value) const;
    private:
        float mData[3];
    };

    Vec3 operator+(float value, const Vec3& vec);
    Vec3 operator-(float value, const Vec3& vec);
    Vec3 operator*(float value, const Vec3& vec);
    Vec3 operator/(float value, const Vec3& vec);

    std::ostream& operator<<(std::ostream& out, const Vec3& vec);
}