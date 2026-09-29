#pragma once
#include "FrameBuffer.h"
#include "Color.h"
#include "Geometry.h"

struct RasterVertex{
    Point position;
    Color color;
    RasterVertex(const Point& position,const Color& color):position(position),color(color){}
};

struct RasterTriangle{
    RasterVertex a;
    RasterVertex b;
    RasterVertex c;
    RasterTriangle(const RasterVertex& a,const RasterVertex& b,const RasterVertex& c):a(a),b(b),c(c){}
};

class Rasterizer{
    private:
        FrameBuffer& framebuffer;
    public:
        Rasterizer(FrameBuffer& fb);
        void drawTriangle(const RasterTriangle& t);
};