#include "Vector3.h"
#include <cmath>
#include <stdexcept>

Vector3 Vector3::operator+(const Vector3& other) const{
    return Vector3(x+other.x,y+other.y,z+other.z);
}

Vector3 Vector3::operator-(const Vector3& other) const{
    return Vector3(x-other.x,y-other.y,z-other.z);
}

Vector3 Vector3::operator*(double scalar) const{
    return Vector3(x*scalar,y*scalar,z*scalar);
}

double dot(const Vector3& a,const Vector3& b){
    return a.x*b.x+a.y*b.y+a.z*b.z;
}

Vector3 cross(const Vector3& a,const Vector3& b){
    return Vector3(
        a.y*b.z-a.z*b.y,
        a.z*b.x-a.x*b.z,
        a.x*b.y-a.y*b.x
    );
}

double length(const Vector3& vector){
    return std::sqrt(dot(vector,vector));
}

Vector3 normalize(const Vector3& vector){
    double len=length(vector);
    if(len<1e-12){
        throw std::invalid_argument("Cannot normalize a zero vector");
    }
    return vector*(1.0/len);
}
