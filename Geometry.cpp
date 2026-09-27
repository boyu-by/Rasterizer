#include "Geometry.h"
#include <cmath>
#include <stdexcept>

double cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

bool insideTriangle(const Point& p, const Triangle& t) {
    double cross1 = cross(t.a, t.b, p);
    double cross2 = cross(t.b, t.c, p);
    double cross3 = cross(t.c, t.a, p);
    return (cross1 >= 0 && cross2 >= 0 && cross3 >= 0) || (cross1 <= 0 && cross2 <= 0 && cross3 <= 0);
}

Point operator*(const Matrix4& matrix, const Point& point){
    Vector4 result=matrix*Vector4(point.x,point.y,point.z,1);
    return Point(result.x,result.y,result.z);
}

Triangle operator*(const Matrix4& matrix, const Triangle& triangle){
    return Triangle(matrix*triangle.a,matrix*triangle.b,matrix*triangle.c);
}

Point perspectiveDivide(const Vector4& point){
    if(std::abs(point.w)<1e-12){
        throw std::invalid_argument("cannot divide by zero w");
    }
    return Point(point.x/point.w,point.y/point.w,point.z/point.w);
}
