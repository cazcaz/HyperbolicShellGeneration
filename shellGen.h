#pragma once
#include "point3D.h"
#include <memory>
#include <vector>
#include <eigen3/Eigen/core>

using Eigen::Vector3d;

class ShellGen {
    public:
        ShellGen();
        ~ShellGen();

        void setInitCurve(double radius, Vector3d centre, int resolution);

        void expandCurve(double length, double stiffness = 0.05, double lengthCoef = 100);

        void expandCurveNTimes(int iterations, double length, double stiffness = 0.05, double lengthCoef = 100);
    
    private:
        std::vector<std::vector<Vector3d>> m_surface;
        int m_resolution;
};