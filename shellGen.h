#pragma once
#include <memory>
#include <vector>
#include <Eigen/core>
#include <string>

using Eigen::Vector3d;

class ShellGen {
    public:
        ShellGen();
        ~ShellGen();

        void setInitCurve(double radius, double centreX, double centreY, double centreZ, int resolution);

        void expandCurve(double length, double stiffness = 0.05, double lengthCoef = 100, double desiredCurvature = 0.01);

        void expandCurveNTimes(int iterations, double length, double stiffness = 0.05, double lengthCoef = 100, double desiredCurvature = 0.01);

        void printSurface(std::string& fileName);

    private:
        std::vector<std::vector<Vector3d>> m_surface;
        int m_resolution;
};