#pragma once
#include "Matrix4.h"
struct Point{
    double x;
    double y;
    double z;
    Point():x(0),y(0),z(0){}
    Point(double x, double y, double z=0):x(x),y(y),z(z){}
};
struct Triangle{
    Point a;
    Point b;
    Point c;
    Triangle(Point a, Point b, Point c):a(a),b(b),c(c){}
    Triangle():a(Point(0,0)),b(Point(0,0)),c(Point(0,0)){}
};
double cross(const Point& a, const Point& b, const Point& c);
bool insideTriangle(const Point& p, const Triangle& t);
Point operator*(const Matrix4& matrix, const Point& point);
Triangle operator*(const Matrix4& matrix, const Triangle& triangle);
Point perspectiveDivide(const Vector4& point);
