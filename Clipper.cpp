#include "Clipper.h"
#include <algorithm>
enum class ClipPlane{
    Left,
    Right,
    Bottom,
    Top,
    Near,
    Far
};
// 返回顶点相对裁剪平面的值：正数在内部，负数在外部
double planeValue(const ClipVertex& vertex, ClipPlane plane) {
    const Vector4& pos = vertex.position;
    switch(plane){
        case ClipPlane::Left:
            return pos.x + pos.w;
        case ClipPlane::Right:
            return -pos.x + pos.w;
        case ClipPlane::Bottom:
            return pos.y + pos.w;
        case ClipPlane::Top:
            return -pos.y + pos.w;
        case ClipPlane::Near:
            return pos.z + pos.w;
        case ClipPlane::Far:
            return -pos.z + pos.w;
    }
    return 0;
}

ClipVertex interpolateVertex(const ClipVertex& start,const ClipVertex& end,double t){
    const Vector4& a=start.position;
    const Vector4& b=end.position;

    Vector4 position(a.x+t*(b.x-a.x),a.y+t*(b.y-a.y),a.z+t*(b.z-a.z),a.w+t*(b.w-a.w));

    Color color(
        (int)(start.color.r+t*(end.color.r-start.color.r)),
        (int)(start.color.g+t*(end.color.g-start.color.g)),
        (int)(start.color.b+t*(end.color.b-start.color.b))
    );
    return ClipVertex(position,color);
}

//计算边与指定裁剪平面的交点
ClipVertex intersectPlane(const ClipVertex& start, const ClipVertex& end, ClipPlane plane) {
    double startV=planeValue(start, plane);
    double endV=planeValue(end, plane);
    double t=startV/(startV-endV);
    t=std::clamp(t,0.0,1.0);
    return interpolateVertex(start, end, t);
}

bool insidePlane(const ClipVertex& vertex, ClipPlane plane){
    return planeValue(vertex, plane)>=-1e-12;
}

std::vector<ClipVertex> clipAgainstPlane(const std::vector<ClipVertex>& input,ClipPlane plane){
    std::vector<ClipVertex> output;
    if(input.empty()) return output;
    //从最后一个顶点到第一个顶点，保证闭合
    ClipVertex start=input.back();
    bool startInside=insidePlane(start,plane);

    for(const ClipVertex& end:input){
        bool endInside=insidePlane(end,plane);
        if(startInside && endInside){
            //两个顶点都在平面内，直接添加end
            output.push_back(end);
        }else if(startInside && !endInside){
            //start在平面内，end在平面外，添加交点
            output.push_back(intersectPlane(start,end,plane));
        }else if(!startInside && endInside){
            //start在平面外，end在平面内，添加交点和end
            output.push_back(intersectPlane(start,end,plane));
            output.push_back(end);
        }
        start=end;
        startInside=endInside;
    }
    return output;
}

std::vector<ClipTriangle> clipTriangle(const ClipTriangle& triangle){
    std::vector<ClipVertex> vertices={triangle.a,triangle.b,triangle.c};
    std::vector<ClipPlane> planes={
        ClipPlane::Left,
        ClipPlane::Right,
        ClipPlane::Bottom,
        ClipPlane::Top,
        ClipPlane::Near,
        ClipPlane::Far
    };
    for(ClipPlane plane:planes){
        vertices=clipAgainstPlane(vertices,plane);
        if(vertices.size()<3) return {};
    }
    std::vector<ClipTriangle> result;
    for(size_t i=1;i+1<vertices.size();i++){
        result.emplace_back(vertices[0],vertices[i],vertices[i+1]);
    }
    return result;
}
