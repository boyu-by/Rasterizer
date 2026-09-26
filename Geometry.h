#pragma once
#include "Matrix4.h"
struct Point{
    double x;
    double y;
    Point():x(0),y(0){}
    Point(double x, double y):x(x),y(y){}
};
struct Triangle{
    Point a;
    Point b;
    Point c;
    Triangle(Point a, Point b, Point c):a(a),b(b),c(c){}
    Triangle():a(Point(0,0)),b(Point(0,0)),c(Point(0,0)){}
};
//计算叉积
double cross(const Point& a, const Point& b, const Point& c);
//判断点是否在三角形内部
bool insideTriangle(const Point& p, const Triangle& t);
//对点和三角形进行矩阵变换
Point operator*(const Matrix4& matrix, const Point& point);
Triangle operator*(const Matrix4& matrix, const Triangle& triangle);
