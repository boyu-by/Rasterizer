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
                    double d=bary.alpha*t.a.inverseW+bary.beta*t.b.inverseW+bary.gamma*t.c.inverseW;
                    if(abs(d)<eps){
                        continue;
                    }
                    double alpha=bary.alpha*t.a.inverseW/d;
                    double beta=bary.beta*t.b.inverseW/d;
                    double gamma=bary.gamma*t.c.inverseW/d;

                    double u=alpha*t.a.uv.x+beta*t.b.uv.x+gamma*t.c.uv.x;
                    double v=alpha*t.a.uv.y+beta*t.b.uv.y+gamma*t.c.uv.y;
                    int checkU=(int)floor(u*8);
                    int checkV=(int)floor(v*8);
                    Color textureColor=(checkU+checkV)%2==0?Color(255,255,255):Color(40,40,40);

                    double vertexRed=alpha*t.a.color.r+beta*t.b.color.r+gamma*t.c.color.r;
                    double vertexGreen=alpha*t.a.color.g+beta*t.b.color.g+gamma*t.c.color.g;
                    double vertexBlue=alpha*t.a.color.b+beta*t.b.color.b+gamma*t.c.color.b;

                    red+=vertexRed*textureColor.r/255.0;
                    green+=vertexGreen*textureColor.g/255.0;
                    blue+=vertexBlue*textureColor.b/255.0;
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
