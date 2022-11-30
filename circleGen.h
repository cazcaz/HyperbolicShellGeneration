#pragma once

#include "curve.h"
#include "Point3D.h"

class CircleGen {
    public:
        CircleGen();
        ~CircleGen();

        Curve makeCircle(double radius, Point3D centre, int resolution);
};