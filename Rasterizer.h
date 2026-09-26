#pragma once
#include "FrameBuffer.h"
#include "Color.h"
#include "Geometry.h"
class Rasterizer {
    private:
        FrameBuffer& framebuffer;
    public:
        Rasterizer(FrameBuffer& fb);
        void drawTriangle(Triangle& t, Color c);
};
