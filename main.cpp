#include <iostream>
#include <vector>
#include <fstream>
#include "Color.h"
#include "FrameBuffer.h"
#include "Geometry.h"
#include "Rasterizer.h"
#include "Transform.h"
using namespace std;
int main(){
    Triangle t(Point(100,100), Point(200,100), Point(150,200));
    Matrix4 model=
        translate(150,150,0)*
        rotateZ(30)*
        scale(1.2,1.2,1)*
        translate(-150,-150,0);
    Triangle transformedTriangle=model*t;
    FrameBuffer fb(800,600);
    Rasterizer r(fb);
    r.drawTriangle(transformedTriangle, Color(255, 0, 0));
    fb.savePPM("out.ppm");
    return 0;
}
