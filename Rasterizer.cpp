#include "Rasterizer.h"
#include <algorithm>
#include <cmath>

Rasterizer::Rasterizer(FrameBuffer& fb):framebuffer(fb){}

Vector2 interpolateUV(const Point& point,const RasterTriangle& t,const Triangle& geometry){
    Barycentric bary=barycentric(point,geometry);
    double d=bary.alpha*t.a.inverseW+bary.beta*t.b.inverseW+bary.gamma*t.c.inverseW;
    double alpha=bary.alpha*t.a.inverseW/d;
    double beta=bary.beta*t.b.inverseW/d;
    double gamma=bary.gamma*t.c.inverseW/d;
    return Vector2(
        alpha*t.a.uv.x+beta*t.b.uv.x+gamma*t.c.uv.x,
        alpha*t.a.uv.y+beta*t.b.uv.y+gamma*t.c.uv.y
    );
}

void Rasterizer::drawTriangle(const RasterTriangle& t,const Texture& texture,bool blend,bool depthWrite){
    Triangle geometry(
        t.a.position,
        t.b.position,
        t.c.position
    );

    //屏幕空间中顺时针为正面
    double area=cross(geometry.a,geometry.b,geometry.c);
    if(area>=-1e-12){
        return;
    }

    //计算三角形的包围盒
    int xmin=static_cast<int>(floor(min({geometry.a.x,geometry.b.x,geometry.c.x})));
    int xmax=static_cast<int>(ceil(max({geometry.a.x,geometry.b.x,geometry.c.x})));
    int ymin=static_cast<int>(floor(min({geometry.a.y,geometry.b.y,geometry.c.y})));
    int ymax=static_cast<int>(ceil(max({geometry.a.y,geometry.b.y,geometry.c.y})));

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
                    Vector2 uvRight=interpolateUV(Point(samples[i].x+1,samples[i].y),t,geometry);
                    Vector2 uvDown=interpolateUV(Point(samples[i].x,samples[i].y+1),t,geometry);
                    Color textureColor=texture.sampleTrilinear(
                        u,v,
                        uvRight.x-u,uvRight.y-v,
                        uvDown.x-u,uvDown.y-v
                    );

                    double vertexRed=alpha*t.a.color.r+beta*t.b.color.r+gamma*t.c.color.r;
                    double vertexGreen=alpha*t.a.color.g+beta*t.b.color.g+gamma*t.c.color.g;
                    double vertexBlue=alpha*t.a.color.b+beta*t.b.color.b+gamma*t.c.color.b;
                    double vertexAlpha=alpha*t.a.color.a+beta*t.b.color.a+gamma*t.c.color.a;

                    double depth=
                        bary.alpha*t.a.position.z+
                        bary.beta*t.b.position.z+
                        bary.gamma*t.c.position.z;
                    depth=(depth+1.0)/2.0;

                    Color sampleColor(
                        (int)(vertexRed*textureColor.r/255.0),
                        (int)(vertexGreen*textureColor.g/255.0),
                        (int)(vertexBlue*textureColor.b/255.0),
                        (int)(vertexAlpha*textureColor.a/255.0)
                    );
                    framebuffer.setSample(x,y,i,depth,sampleColor,blend,depthWrite);
                }
            }
        }
    }
}
