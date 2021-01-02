#pragma once

#include <initializer_list>
#include <iostream>

#include "Vec3.h"

namespace Math {
    class Mat3 {
    public:
        Mat3();
        Mat3(float value);
        Mat3(std::initializer_list<float> list);
        Mat3(std::initializer_list<Vec3> list);

        const float* data() const;
        float* data();
        unsigned int size() const;

        const Vec3& operator[](unsigned int i) const;
        Vec3& operator[](unsigned int i);

        Mat3 operator-() const;
        Mat3 operator+(const Mat3& other) const;
        Mat3 operator-(const Mat3& other) const;
        Mat3 operator*(const Mat3& other) const;
        Mat3 operator/(const Mat3& other) const;

        Mat3 operator+(float value) const;
        Mat3 operator-(float value) const;
        Mat3 operator*(float value) const;
        Mat3 operator/(float value) const;

        Vec3 operator*(const Vec3& vec) const;

        Mat3 Transpose() const;
        Mat3 Dot(const Mat3& mat) const;
        Vec3 Dot(const Vec3& vec) const;
    private:
        Vec3 mData[3];
    };

    Mat3 operator+(float value, const Mat3& mat);
    Mat3 operator-(float value, const Mat3& mat);
    Mat3 operator*(float value, const Mat3& mat);
    Mat3 operator/(float value, const Mat3& mat);

    std::ostream& operator<<(std::ostream& out, const Mat3& mat);
}