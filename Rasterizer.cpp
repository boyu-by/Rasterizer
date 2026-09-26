#include "Rasterizer.h"
#include <algorithm>
#include <cmath>
Rasterizer::Rasterizer(FrameBuffer& fb) : framebuffer(fb) {}
void Rasterizer::drawTriangle(Triangle& t, Color c) {
    //计算三角形的包围盒
    int xmin = floor(min({t.a.x,t.b.x,t.c.x}));
    int xmax = ceil(max({t.a.x,t.b.x,t.c.x}));
    int ymin = floor(min({t.a.y, t.b.y, t.c.y}));
    int ymax = ceil(max({t.a.y, t.b.y, t.c.y}));
    xmin = max(xmin,0);
    xmax = min(xmax,framebuffer.getWidth()-1);
    ymin = max(ymin,0);
    ymax = min(ymax,framebuffer.getHeight()-1);

    for(int y=ymin;y<=ymax;y++){
        for(int x=xmin;x<=xmax;x++){
            //超采样4个点
            Point samples[4];
            samples[0] = Point(x + 0.25, y + 0.25);
            samples[1] = Point(x + 0.75, y + 0.25);
            samples[2] = Point(x + 0.25, y + 0.75);
            samples[3] = Point(x + 0.75, y + 0.75);
            int cnt = 0;
            for(int i=0;i<4;i++){
                if(insideTriangle(samples[i],t)){
                    cnt++;
                }
            }
            double rate = cnt / 4.0;
            Color finalColor((int)(c.r * rate), (int)(c.g * rate), (int)(c.b * rate));
            framebuffer.setPixel(x,y,finalColor);
        }
    }
}