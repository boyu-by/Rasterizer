#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include "Color.h"
#include "FrameBuffer.h"
#include "Geometry.h"
#include "Rasterizer.h"
#include "Transform.h"
#include "Clipper.h"
#include "Texture.h"
using namespace std;

double clipDepth(const ClipTriangle& triangle){
    return (
        triangle.a.position.z/triangle.a.position.w+
        triangle.b.position.z/triangle.b.position.w+
        triangle.c.position.z/triangle.c.position.w
    )/3.0;
}

bool farther(const ClipTriangle& a,const ClipTriangle& b){
    return clipDepth(a)>clipDepth(b);
}

ClipTriangle toClipTriangle(
    const Triangle& triangle,
    const Matrix4& mvp,
    const Color& colorA,
    const Color& colorB,
    const Color& colorC
){
    return ClipTriangle(
        ClipVertex(
            mvp*Vector4(triangle.a.x,triangle.a.y,triangle.a.z,1),
            colorA,
            Vector2(0,0)
        ),
        ClipVertex(
            mvp*Vector4(triangle.b.x,triangle.b.y,triangle.b.z,1),
            colorB,
            Vector2(1,0)
        ),
        ClipVertex(
            mvp*Vector4(triangle.c.x,triangle.c.y,triangle.c.z,1),
            colorC,
            Vector2(0.5,1)
        )
    );
}

void drawTriangles(
    const std::vector<ClipTriangle>& triangles,
    Rasterizer& rasterizer,
    const Texture& texture,
    const Matrix4& viewportTransform,
    bool blend,
    bool depthWrite
){
    for(const ClipTriangle& clipped:triangles){
        Triangle triangleNDC(
            perspectiveDivide(clipped.a.position),
            perspectiveDivide(clipped.b.position),
            perspectiveDivide(clipped.c.position)
        );

        Triangle triangleSS=
            viewportTransform*triangleNDC;

        RasterTriangle rasterTriangle{
            RasterVertex(triangleSS.a,clipped.a.color,clipped.a.uv,1.0/clipped.a.position.w),
            RasterVertex(triangleSS.b,clipped.b.color,clipped.b.uv,1.0/clipped.b.position.w),
            RasterVertex(triangleSS.c,clipped.c.color,clipped.c.uv,1.0/clipped.c.position.w)
        };

        rasterizer.drawTriangle(rasterTriangle,texture,blend,depthWrite);
    }
}

int main(){
    const int width=800;
    const int height=600;

    Matrix4 model=rotateY(30)*scale(2,2,2);
    Matrix4 view=lookAt(Vector3(0,0,5),Vector3(0,0,0),Vector3(0,1,0));
    Matrix4 projection=perspective(60,static_cast<double>(width)/height,0.1,100);
    Matrix4 mvp=projection*view*model;

    Triangle frontTriangleOS(Point(-1,-1,0),Point(1,-1,0),Point(0,1,0));
    Triangle backTriangleOS(Point(-1.2,-0.8,-0.8),Point(0.8,-0.8,-0.8),Point(-0.2,1.2,-0.8));
    Triangle transparentTriangleOS(Point(-0.7,-0.9,0.6),Point(1.3,-0.9,0.6),Point(0.3,1.1,0.6));

    ClipTriangle frontTriangleClip=toClipTriangle(
        frontTriangleOS,mvp,
        Color(255,0,0),
        Color(0,255,0),
        Color(0,0,255)
    );
    ClipTriangle backTriangleClip=toClipTriangle(
        backTriangleOS,mvp,
        Color(80,150,255),
        Color(80,150,255),
        Color(80,150,255)
    );
    ClipTriangle transparentTriangleClip=toClipTriangle(
        transparentTriangleOS,mvp,
        Color(255,160,40,128),
        Color(255,160,40,128),
        Color(255,160,40,128)
    );

    std::vector<ClipTriangle> clippedTriangles=
        clipTriangle(frontTriangleClip);
    std::vector<ClipTriangle> backClippedTriangles=
        clipTriangle(backTriangleClip);
    clippedTriangles.insert(
        clippedTriangles.end(),
        backClippedTriangles.begin(),
        backClippedTriangles.end()
    );

    std::vector<ClipTriangle> transparentClippedTriangles=
        clipTriangle(transparentTriangleClip);
    std::sort(
        transparentClippedTriangles.begin(),
        transparentClippedTriangles.end(),
        farther
    );

    FrameBuffer fb(width,height);
    Rasterizer r(fb);
    Texture texture("texture.ppm");
    Matrix4 viewportTransform=viewport(width,height);

    drawTriangles(clippedTriangles,r,texture,viewportTransform,false,true);
    drawTriangles(transparentClippedTriangles,r,texture,viewportTransform,true,false);

    fb.savePPM("out.ppm");
    return 0;
}
