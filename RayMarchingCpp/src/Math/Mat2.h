#pragma once

#include <initializer_list>
#include <iostream>

#include "Vec2.h"

namespace Math {
    class Mat2 {
    public:
        Mat2();
        explicit Mat2(float value);
        Mat2(std::initializer_list<float> list);
        Mat2(std::initializer_list<Vec2> list);

        const float* data() const;
        float* data();
        unsigned int size() const;

        const Vec2& operator[](unsigned int i) const;
        Vec2& operator[](unsigned int i);

        Mat2 operator-() const;
        Mat2 operator+(const Mat2& other) const;
        Mat2 operator-(const Mat2& other) const;
        Mat2 operator*(const Mat2& other) const;
        Mat2 operator/(const Mat2& other) const;

        Mat2 operator+(float value) const;
        Mat2 operator-(float value) const;
        Mat2 operator*(float value) const;
        Mat2 operator/(float value) const;

        Vec2 operator*(const Vec2& vec) const;

        Mat2 Transpose() const;
        Mat2 Dot(const Mat2& mat) const;
        Vec2 Dot(const Vec2& vec) const;
    private:
        Vec2 mData[2];
    };

    Mat2 operator+(float value, const Mat2& mat);
    Mat2 operator-(float value, const Mat2& mat);
    Mat2 operator*(float value, const Mat2& mat);
    Mat2 operator/(float value, const Mat2& mat);

    std::ostream& operator<<(std::ostream& out, const Mat2& mat);
}