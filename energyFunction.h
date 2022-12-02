#pragma once

#include <Eigen/Core>
#include <vector>

using Eigen::Vector3d;
using Eigen::VectorXd;

class EnergyFunction {
    public:
        EnergyFunction(std::vector<Vector3d>& currentCurve, std::vector<Vector3d>& normals, std::vector<Vector3d>& binormals, double length, double stiffnessCoef, double lengthPunishCoef, double radialDist, double initialDist);
        ~EnergyFunction();

        double operator()(const VectorXd& inputs, VectorXd& derivatives);
        VectorXd growthEnergyDerivative(VectorXd& inputs);
        Vector3d normalVecDeriv(Vector3d& a, Vector3d& b, Vector3d& da, Vector3d& db);
        double dxDij(double x, double xdij, Vector3d p1, Vector3d p2, Vector3d p3, Vector3d dp1, Vector3d dp2, Vector3d dp3);
        int correctIndex(int index);
        double lengthFunction(double t, double t0);
    private:
        std::vector<Vector3d> m_currentCurve;
        std::vector<Vector3d> m_normals;
        std::vector<Vector3d> m_binormals;
        double m_length;
        double m_stiffnessCoef;
        double m_lengthPunishCoef;
        double m_radialDist;
        double m_initialDist;
        int m_resolution;
};