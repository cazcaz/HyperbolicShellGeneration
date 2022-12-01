#pragma once
#include "point3D.h"
#include <memory>
#include <vector>

class ShellGen {
    public:
        ShellGen();
        ~ShellGen();

        void setInitCurve(double radius, Point3D centre, int resolution);

        void expandCurve(double length, double stiffness = 0.05, double lengthCoef = 100);

        void expandCurveNTimes(int iterations, double length, double stiffness = 0.05, double lengthCoef = 100);
    
        std::vector<std::vector<Point3D>> getSurface();
    private:
        std::vector<std::vector<Point3D>> m_surface;
        int m_resolution;
};