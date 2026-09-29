#include "Rasterizer.h"
#include <algorithm>
#include <cmath>

Rasterizer::Rasterizer(FrameBuffer& fb):framebuffer(fb){}

void Rasterizer::drawTriangle(const RasterTriangle& t){
    Triangle geometry(
        t.a.position,
        t.b.position,
        t.c.position
    );

    if(abs(cross(geometry.a,geometry.b,geometry.c))<1e-12){
        return;
    }

    //计算三角形的包围盒
    int xmin=floor(min({geometry.a.x,geometry.b.x,geometry.c.x}));
    int xmax=ceil(max({geometry.a.x,geometry.b.x,geometry.c.x}));
    int ymin=floor(min({geometry.a.y,geometry.b.y,geometry.c.y}));
    int ymax=ceil(max({geometry.a.y,geometry.b.y,geometry.c.y}));

    xmin=max(xmin,0);
    xmax=min(xmax,framebuffer.getWidth()-1);
    ymin=max(ymin,0);
    ymax=min(ymax,framebuffer.getHeight()-1);

    const double eps=1e-12;

    for(int y=ymin;y<=ymax;y++){
        for(int x=xmin;x<=xmax;x++){
            //超采样4个点
            Point samples[4];
            samples[0]=Point(x+0.25,y+0.25);
            samples[1]=Point(x+0.75,y+0.25);
            samples[2]=Point(x+0.25,y+0.75);
            samples[3]=Point(x+0.75,y+0.75);

            double red=0;
            double green=0;
            double blue=0;
            int cnt=0;

            for(int i=0;i<4;i++){
                Barycentric bary=barycentric(samples[i],geometry);

                if(bary.alpha>=-eps&&bary.beta>=-eps&&bary.gamma>=-eps){
                    red+=
                        bary.alpha*t.a.color.r+
                        bary.beta*t.b.color.r+
                        bary.gamma*t.c.color.r;

                    green+=
                        bary.alpha*t.a.color.g+
                        bary.beta*t.b.color.g+
                        bary.gamma*t.c.color.g;

                    blue+=
                        bary.alpha*t.a.color.b+
                        bary.beta*t.b.color.b+
                        bary.gamma*t.c.color.b;

                    cnt++;
                }
            }

            if(cnt==0){
                continue;
            }

            Color finalColor(
                (int)(red/4.0),
                (int)(green/4.0),
                (int)(blue/4.0)
            );

            framebuffer.setPixel(x,y,finalColor);
        }
    }
}