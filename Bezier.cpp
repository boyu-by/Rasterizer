#include "Bezier.h"

Vector3 bezierCurve(
    const Vector3& p0,
    const Vector3& p1,
    const Vector3& p2,
    const Vector3& p3,
    double t
) {
    double s =1.0-t;
    return p0*(s*s*s)+ p1*(3.0*s*s*t)+ p2*(3.0*s*t*t)+p3*(t*t*t);
}

Vector3 bezierCurveDerivative(
    const Vector3& p0,
    const Vector3& p1,
    const Vector3& p2,
    const Vector3& p3,
    double t
) {
    double s = 1.0 - t;
    return (p1 - p0)*(3.0*s*s)+(p2 - p1)*(6.0*s*t)+(p3 - p2)*(3.0*t*t);
}

Vector3 bezierSurface(
    const Vector3 controlPoints[4][4],
    double u,
    double v
) {
    Vector3 temp[4];
    for (int i = 0; i < 4; i++) {
        temp[i] = bezierCurve(
            controlPoints[i][0],
            controlPoints[i][1],
            controlPoints[i][2],
            controlPoints[i][3],
            u
        );
    }
    return bezierCurve(
        temp[0],
        temp[1],
        temp[2],
        temp[3],
        v
    );
}