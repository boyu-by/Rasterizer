#include "Transform.h"
#include <cmath>
#include <stdexcept>

Matrix4 translate(double x,double y,double z){
    Matrix4 result=Matrix4::identity();
    result.m[0][3]=x;
    result.m[1][3]=y;
    result.m[2][3]=z;
    return result;
}

Matrix4 scale(double x,double y,double z){
    Matrix4 result=Matrix4::identity();
    result.m[0][0]=x;
    result.m[1][1]=y;
    result.m[2][2]=z;
    return result;
}

Matrix4 rotateX(double angle){
    double radian=angle*std::acos(-1.0)/180.0;
    double c=std::cos(radian);
    double s=std::sin(radian);
    Matrix4 result=Matrix4::identity();
    result.m[1][1]=c;
    result.m[1][2]=-s;
    result.m[2][1]=s;
    result.m[2][2]=c;
    return result;
}

Matrix4 rotateY(double angle){
    double radian=angle*std::acos(-1.0)/180.0;
    double c=std::cos(radian);
    double s=std::sin(radian);
    Matrix4 result=Matrix4::identity();
    result.m[0][0]=c;
    result.m[0][2]=s;
    result.m[2][0]=-s;
    result.m[2][2]=c;
    return result;
}

Matrix4 rotateZ(double angle){
    double radian=angle*std::acos(-1.0)/180.0;
    double c=std::cos(radian);
    double s=std::sin(radian);
    Matrix4 result=Matrix4::identity();
    result.m[0][0]=c;
    result.m[0][1]=-s;
    result.m[1][0]=s;
    result.m[1][1]=c;
    return result;
}

Matrix4 lookAt(const Vector3& eye,const Vector3& center,const Vector3& up){
    Vector3 forward=center-eye;
    if(length(forward)<1e-12){
        throw std::invalid_argument("eye and center cannot be the same");
    }
    forward=normalize(forward);

    Vector3 right=cross(forward,up);
    if(length(right)<1e-12){
        throw std::invalid_argument("up cannot be parallel to the view direction");
    }
    right=normalize(right);
    Vector3 cameraUp=cross(right,forward);

    Matrix4 result=Matrix4::identity();
    result.m[0][0]=right.x;
    result.m[0][1]=right.y;
    result.m[0][2]=right.z;
    result.m[0][3]=-dot(right,eye);

    result.m[1][0]=cameraUp.x;
    result.m[1][1]=cameraUp.y;
    result.m[1][2]=cameraUp.z;
    result.m[1][3]=-dot(cameraUp,eye);

    result.m[2][0]=-forward.x;
    result.m[2][1]=-forward.y;
    result.m[2][2]=-forward.z;
    result.m[2][3]=dot(forward,eye);
    return result;
}

Matrix4 perspective(double fovY,double aspect,double nearPlane,double farPlane){
    if(fovY<=0||fovY>=180){
        throw std::invalid_argument("fovY must be between 0 and 180");
    }
    if(aspect<=0||nearPlane<=0||farPlane<=nearPlane){
        throw std::invalid_argument("invalid perspective parameters");
    }
    double radian=fovY*std::acos(-1.0)/180.0;
    double s=1.0/std::tan(radian/2.0);
    Matrix4 result;
    result.m[0][0]=s/aspect;
    result.m[1][1]=s;
    result.m[2][2]=-(farPlane+nearPlane)/(farPlane-nearPlane);
    result.m[2][3]=-2*farPlane*nearPlane/(farPlane-nearPlane);
    result.m[3][2]=-1;
    return result;
}

Matrix4 viewport(double width,double height){
    return translate(width/2.0,height/2.0,0)*scale(width/2.0,-height/2.0,1);
}
