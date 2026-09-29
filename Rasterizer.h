#pragma once
#include "FrameBuffer.h"
#include "Color.h"
#include "Geometry.h"
#include "Vector2.h"

struct RasterVertex{
    Point position;
    Color color;
    Vector2 uv;
    double inverseW; //Clip Space中w的倒数，用于透视正确插值
    RasterVertex(const Point& position,const Color& color,const Vector2& uv,double inverseW):position(position),color(color),uv(uv),inverseW(inverseW){}
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
