#pragma once
#include "FrameBuffer.h"
#include "Color.h"
#include "Geometry.h"

struct RasterVertex{
    Point position;
    Color color;
    double inverseW; //Clip Space中w的倒数，用于透视正确插值
    RasterVertex(const Point& position,const Color& color,double inverseW):position(position),color(color),inverseW(inverseW){}
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