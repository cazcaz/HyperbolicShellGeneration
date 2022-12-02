#pragma once

#include "Point3D.h"
#include <vector>
#include <Eigen/core>

using  Eigen::Vector3d;

class CircleGen {
    public:
        CircleGen();
        ~CircleGen();

        void makeCircle(double radius, Vector3d centre, int resolution, std::vector<Vector3d>& curve);
};