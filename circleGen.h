#pragma once

#include "Point3D.h"
#include <vector>

class CircleGen {
    public:
        CircleGen();
        ~CircleGen();

        void makeCircle(double radius, Point3D centre, int resolution, std::vector<Point3D>& curve);
};