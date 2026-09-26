#pragma once
#include "Matrix4.h"
#include "Vector3.h"

Matrix4 translate(double x,double y,double z);
Matrix4 scale(double x,double y,double z);
Matrix4 rotateX(double angle);
Matrix4 rotateY(double angle);
Matrix4 rotateZ(double angle);
Matrix4 lookAt(const Vector3& eye,const Vector3& center,const Vector3& up);
