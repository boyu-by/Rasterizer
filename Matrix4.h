#pragma once
#include "Vector4.h"
class Matrix4{
    public:
        double m[4][4];
        Matrix4();
        static Matrix4 identity();
        Matrix4 operator*(const Matrix4& other) const;
        Vector4 operator*(const Vector4& vector) const;
};
