#pragma once
#include <vector>
#include "Vector4.h"
#include "Vector2.h"
#include "Color.h"

struct ClipVertex{
    Vector4 position;
    Color color;
    Vector2 uv;
    ClipVertex(const Vector4& position,const Color& color,const Vector2& uv):position(position),color(color),uv(uv){}
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
