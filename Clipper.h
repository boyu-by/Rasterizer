#pragma once
#include <vector>
#include "Vector4.h"
#include "Color.h"

struct ClipVertex{
    Vector4 position;
    Color color;
    ClipVertex(const Vector4& position,const Color& color):position(position),color(color){}
};

struct ClipTriangle{
    ClipVertex a;
    ClipVertex b;
    ClipVertex c;

    ClipTriangle(const ClipVertex& a, const ClipVertex& b, const ClipVertex& c):a(a),b(b),c(c){};
};

std::vector<ClipTriangle> clipTriangle(
    const ClipTriangle& triangle
);