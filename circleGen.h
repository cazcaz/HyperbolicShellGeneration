#pragma once

#include "curve.h"
#include "Point3D.h"
#include <memory>

class CircleGen {
    public:
        CircleGen();
        ~CircleGen();

        std::unique_ptr<Curve> makeCircle(double radius, Point3D centre, int resolution);
};