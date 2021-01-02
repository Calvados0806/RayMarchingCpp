#pragma once

#include <initializer_list>
#include <iostream>

namespace Math {
    class Mat2;

    class Vec2 {
    public:
        Vec2();
        Vec2(std::initializer_list<float> list);
        explicit Vec2(float value);
        Vec2(float v1, float v2);
        Vec2(float arr[2]);

        float& x();
        float  x() const;
        float& y();
        float  y() const;

        const float* data() const;
        float* data();
        unsigned int size() const;

        float operator[](unsigned int i) const;
        float& operator[](unsigned int i);

        Vec2 operator-() const;
        Vec2 operator+(const Vec2& other) const;
        Vec2 operator-(const Vec2& other) const;
        /*
            Component-wise multiplication
        */
        Vec2 operator*(const Vec2& other) const;
        /*
            Component-wise division
        */
        Vec2 operator/(const Vec2& other) const;

        Vec2 operator+(float value) const;
        Vec2 operator-(float value) const;
        Vec2 operator*(float value) const;
        Vec2 operator/(float value) const;

        float Dot(const Vec2& other) const;
        Vec2 Dot(const Mat2& mat) const;
        Vec2 Normalize() const;
        float Magnitude() const;
        /*
            Same as .Magnitude(), but without calculating sqrt()
        */
        float MagnitudeSquared() const;
        Vec2 Translate(const Vec2& shift) const;
        Vec2 Rotate(float angle) const;
        Vec2 Scale(float value) const;
    private:
        float mData[2];
    };

    Vec2 operator+(float value, const Vec2& vec);
    Vec2 operator-(float value, const Vec2& vec);
    Vec2 operator*(float value, const Vec2& vec);
    Vec2 operator/(float value, const Vec2& vec);

    std::ostream& operator<<(std::ostream& out, const Vec2& vec);
}