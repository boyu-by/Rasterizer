#include <iostream>
#include <vector>
#include <fstream>
#include "Color.h"
#include "FrameBuffer.h"
#include "Geometry.h"
#include "Rasterizer.h"
#include "Transform.h"
#include "Clipper.h"
using namespace std;

int main(){
    const int width=800;
    const int height=600;

    Triangle triangleOS(Point(-1,-1,0),Point(1,-1,0),Point(0,1,0));
    Matrix4 model=rotateY(30)*scale(2,2,2);
    Matrix4 view=lookAt(Vector3(0,0,5),Vector3(0,0,0),Vector3(0,1,0));
    Matrix4 projection=perspective(60,static_cast<double>(width)/height,0.1,100);
    Matrix4 mvp=projection*view*model;
    Vector4 aClip=mvp*Vector4(
        triangleOS.a.x,
        triangleOS.a.y,
        triangleOS.a.z,
        1
    );
    Vector4 bClip=mvp*Vector4(
        triangleOS.b.x,
        triangleOS.b.y,
        triangleOS.b.z,
        1
    );
    Vector4 cClip=mvp*Vector4(
        triangleOS.c.x,
        triangleOS.c.y,
        triangleOS.c.z,
        1
    );
    ClipTriangle triangleClip{
        ClipVertex(aClip,Color(255,0,0)),
        ClipVertex(bClip,Color(0,255,0)),
        ClipVertex(cClip,Color(0,0,255))
    };

    std::vector<ClipTriangle> clippedTriangles=
        clipTriangle(triangleClip);

    FrameBuffer fb(width,height);
    Rasterizer r(fb);
    Matrix4 viewportTransform=viewport(width,height);

    for(const ClipTriangle& clipped:clippedTriangles){
        Triangle triangleNDC(
            perspectiveDivide(clipped.a.position),
            perspectiveDivide(clipped.b.position),
            perspectiveDivide(clipped.c.position)
        );

        Triangle triangleSS=
            viewportTransform*triangleNDC;

        RasterTriangle rasterTriangle{
            RasterVertex(triangleSS.a,clipped.a.color,1.0/clipped.a.position.w),
            RasterVertex(triangleSS.b,clipped.b.color,1.0/clipped.b.position.w),
            RasterVertex(triangleSS.c,clipped.c.color,1.0/clipped.c.position.w)
        };

        r.drawTriangle(rasterTriangle);
    }

    fb.savePPM("out.ppm");
    return 0;
}
