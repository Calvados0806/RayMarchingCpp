#pragma once

#include <initializer_list>
#include <iostream>

#include "Vec4.h"

namespace Math {
    class Mat4 {
    public:
        Mat4();
        explicit Mat4(float value);
        Mat4(std::initializer_list<float> list);
        Mat4(std::initializer_list<Vec4> list);

        const float* data() const;
        float* data();
        unsigned int size() const;

        const Vec4& operator[](unsigned int i) const;
        Vec4& operator[](unsigned int i);

        Mat4 operator-() const;
        Mat4 operator+(const Mat4& other) const;
        Mat4 operator-(const Mat4& other) const;
        Mat4 operator*(const Mat4& other) const;
        Mat4 operator/(const Mat4& other) const;

        Mat4 operator+(float value) const;
        Mat4 operator-(float value) const;
        Mat4 operator*(float value) const;
        Mat4 operator/(float value) const;

        Vec4 operator*(const Vec4& vec) const;

        Mat4 Transpose() const;
        Mat4 Dot(const Mat4& mat) const;
        Vec4 Dot(const Vec4& vec) const;

        static Mat4 CreateTranslation(float dx, float dy, float dz);
        static Mat4 CreateRotationX(float angle);
        static Mat4 CreateRotationY(float angle);
        static Mat4 CreateRotationZ(float angle);
        static Mat4 CreateScale(float sx, float sy, float sz);
    private:
        Vec4 mData[4];
    };

    Mat4 operator+(float value, const Mat4& mat);
    Mat4 operator-(float value, const Mat4& mat);
    Mat4 operator*(float value, const Mat4& mat);
    Mat4 operator/(float value, const Mat4& mat);

    std::ostream& operator<<(std::ostream& out, const Mat4& mat);
}