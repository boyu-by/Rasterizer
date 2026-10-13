#pragma once
#include "Vector3.h"
#include <vector>
#include "Geometry.h"

Vector3 bezierCurve(
    const Vector3& p0,
    const Vector3& p1,
    const Vector3& p2,
    const Vector3& p3,
    double t
);

Vector3 bezierCurveDerivative(
    const Vector3& p0,
    const Vector3& p1,
    const Vector3& p2,
    const Vector3& p3,
    double t
);

Vector3 bezierSurface(
    const Vector3 controlPoints[4][4],
    double u,
    double v
);