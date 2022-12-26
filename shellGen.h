#pragma once
#include <memory>
#include <vector>
#include <eigen3/Eigen/Core>
#include <string>
#include "shellParams.h"
#include "shellNaming.h"
using Eigen::Vector3d;
using Eigen::VectorXd;

class ShellGen {
    public:
        ShellGen(ShellParams& parameters);
        ~ShellGen();

        void setInitCurve();

        bool expandCurve();

        void expandCurveNTimes();

        int correctIndex(int index);

        void printSurface();

    private:
        struct ShellParams& m_parameters;
        std::vector<std::vector<Vector3d>> m_surface;
        VectorXd m_prevSol;
};