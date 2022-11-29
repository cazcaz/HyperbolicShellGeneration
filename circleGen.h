#pragma once

#include "curve.h"
#include "Point3D.h"

class CircleGen {
    public:
        CircleGen();
        ~CircleGen();

        Curve makeCircle(float radius, Point3D centre, int resolution);
};