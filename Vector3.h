#pragma once

struct Vector3{
    double x;
    double y;
    double z;
    Vector3():x(0),y(0),z(0){}
    Vector3(double x,double y,double z):x(x),y(y),z(z){}
    Vector3 operator+(const Vector3& other) const;
    Vector3 operator-(const Vector3& other) const;
    Vector3 operator*(double scalar) const;
};

double dot(const Vector3& a,const Vector3& b);
Vector3 cross(const Vector3& a,const Vector3& b);
double length(const Vector3& vector);
Vector3 normalize(const Vector3& vector);
