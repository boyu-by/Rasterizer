#include "Geometry.h"
#include <cmath>
#include <stdexcept>

double cross(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}
Barycentric barycentric(const Point& p,const Triangle& t){
    double area=cross(t.a,t.b,t.c);
    if(std::abs(area)<1e-12){
        throw std::invalid_argument("It is a degenerate triangle");
    }
    double alpha=cross(p,t.b,t.c)/area;
    double beta=cross(t.a,p,t.c)/area;
    double gamma=cross(t.a,t.b,p)/area;
    return Barycentric(alpha,beta,gamma);
}

bool insideTriangle(const Point& p, const Triangle& t) {
    Barycentric bary=barycentric(p,t);
    const double esp=1e-12;
    return bary.alpha>=-esp&&bary.beta>=-esp&&bary.gamma>=-esp;
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
