#pragma once
#include <vector>
#include "Vector4.h"

struct ClipVertex{
    Vector4 position;

    ClipVertex(const Vector4& position):position(position){};
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