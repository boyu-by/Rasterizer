#include "Matrix4.h"

Matrix4::Matrix4(){
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            m[i][j]=0;
        }
    }
}

Matrix4 Matrix4::identity(){
    Matrix4 result;
    for(int i=0;i<4;i++){
        result.m[i][i]=1;
    }
    return result;
}

Matrix4 Matrix4::operator*(const Matrix4& other) const{
    Matrix4 result;
    for(int i=0;i<4;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<4;k++){
                result.m[i][j]+=m[i][k]*other.m[k][j];
            }
        }
    }
    return result;
}

Vector4 Matrix4::operator*(const Vector4& vector) const{
    Vector4 result;
    result.x=m[0][0]*vector.x+m[0][1]*vector.y+m[0][2]*vector.z+m[0][3]*vector.w;
    result.y=m[1][0]*vector.x+m[1][1]*vector.y+m[1][2]*vector.z+m[1][3]*vector.w;
    result.z=m[2][0]*vector.x+m[2][1]*vector.y+m[2][2]*vector.z+m[2][3]*vector.w;
    result.w=m[3][0]*vector.x+m[3][1]*vector.y+m[3][2]*vector.z+m[3][3]*vector.w;
    return result;
}
