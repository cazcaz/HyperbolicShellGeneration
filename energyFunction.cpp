#include "energyFunction.h"
#include <cmath>
#include <iostream>

using Eigen::Vector3d;
using Eigen::VectorXd;

EnergyFunction::EnergyFunction(std::vector<Vector3d>& currentCurve,
                               std::vector<Vector3d>& normals,
                               std::vector<Vector3d>& binormals,
                               double length, double stiffnessCoef,
                               double lengthPunishCoef,
                               double radialDist,
                               double initialDist) : 
                               m_currentCurve(currentCurve),
                               m_normals(normals), 
                               m_binormals(binormals),
                               m_length(length),
                               m_stiffnessCoef(stiffnessCoef),
                               m_lengthPunishCoef(lengthPunishCoef),
                               m_radialDist(radialDist),
                               m_initialDist(initialDist) {
    m_resolution = currentCurve.size();
};

EnergyFunction::~EnergyFunction() {};

double EnergyFunction::operator()(const VectorXd& inputs, VectorXd& derivatives){
    std::vector<Vector3d> nextCurve;
    for (int i=0; i<m_resolution; i++) {
        nextCurve.push_back(m_currentCurve[i] + m_length*m_normals[i] + inputs[i]*m_binormals[i]);
    }
    double circumferentialEnergySum = 0;
    double totalLength = 0;
    double radialEnergySum = 0;

    //Find the length of the curve before the main loop so it can be contained in a single loop
    for (int i=0; i<m_resolution;i++){
        Vector3d lastPoint = nextCurve[correctIndex(i-1)];
        Vector3d currentPoint = nextCurve[correctIndex(i)];
        totalLength += (currentPoint-lastPoint).norm();
    }

    //Evaluate energy and derivative in the same loop to save time
    for (int i=0; i<m_resolution;i++){
        //Points on the curve used in each calculation
        Vector3d p1 = nextCurve[correctIndex(i-2)];
        Vector3d p2 = nextCurve[correctIndex(i-1)];
        Vector3d p3 = nextCurve[correctIndex(i)];
        Vector3d p4 = nextCurve[correctIndex(i+1)];
        Vector3d p5 = nextCurve[correctIndex(i+2)];

        //Derivatives of each point w.r.t. ith input
        Vector3d dp1 = Vector3d::Zero(3);
        Vector3d dp2 = Vector3d::Zero(3);
        Vector3d dp3 = m_binormals[i];
        Vector3d dp4 = Vector3d::Zero(3);
        Vector3d dp5 = Vector3d::Zero(3);

        //Cosines of the angles between the rods connecting all 5 points
        double x1 = (p5-p4).normalized().dot((p3-p4).normalized());
        double x2 = (p4-p3).normalized().dot((p2-p3).normalized());
        double x3 = (p3-p2).normalized().dot((p1-p2).normalized());

        //Energy calcs. 
        circumferentialEnergySum += 1/((p2-p3).norm() + (p4-p3).norm()) * std::pow(std::tan(std::acos(x2)),2);
        radialEnergySum += std::pow(inputs[i]/m_length,2);

        //Derivative calculations
        double xdij1 = (p5-p4).normalized().dot(normalVecDeriv(p3,p4,dp3,dp4)) + (p3-p4).normalized().dot(normalVecDeriv(p5,p4,dp5,dp4));
        double xdij2 = (p4-p3).normalized().dot(normalVecDeriv(p2,p3,dp2,dp3)) + (p2-p3).normalized().dot(normalVecDeriv(p4,p3,dp4,dp3));
        double xdij3 = (p3-p2).normalized().dot(normalVecDeriv(p1,p2,dp1,dp2)) + (p1-p2).normalized().dot(normalVecDeriv(p3,p2,dp3,dp2));

        double energyBendDeriv = dxDij(x1, xdij1, p3,p4,p5,dp3,dp4,dp5) + dxDij(x2, xdij2,p2,p3,p4,dp2,dp3,dp4) + dxDij(x3, xdij3, p1,p2,p3,dp1,dp2,dp3);
        double lengthEnergyDeriv = 2 * m_lengthPunishCoef * (totalLength - lengthFunction(m_radialDist, m_initialDist)) * ((dp4-dp3).dot((p4-p3).normalized()) + (dp3-dp2).dot((p3-p2).normalized()));
        double energyRadialBendDeriv = m_stiffnessCoef/std::pow(m_length,3) * inputs[i];
        derivatives[i] = energyBendDeriv + energyRadialBendDeriv + lengthEnergyDeriv;
        
        //Isolated energies for testing
        //derivatives[i] = energyBendDeriv;
        //derivatives[i] = energyRadialBendDeriv;
        //derivatives[i] = lengthEnergyDeriv;
    }

    double lengthEnergy = m_lengthPunishCoef * std::pow(totalLength-lengthFunction(m_radialDist, m_initialDist),2);
    double totalEnergy = m_stiffnessCoef * circumferentialEnergySum + m_stiffnessCoef/(2*m_length) * radialEnergySum + lengthEnergy;
    std::cout <<"Bending Energy: " <<  m_stiffnessCoef * circumferentialEnergySum << "  Radial Bending Energy: " << m_stiffnessCoef/(2*m_length) * radialEnergySum << "  Length Energy: " << lengthEnergy << "  Total Energy: " << totalEnergy << std::endl;
    //Isolated energy derivatives for testing
    //double totalEnergy = m_stiffnessCoef * circumferentialEnergySum;
    //double totalEnergy = m_stiffnessCoef/(2*m_length) * radialEnergySum;
    //double totalEnergy = lengthEnergy;

    return totalEnergy;
};

Vector3d EnergyFunction::normalVecDeriv(Vector3d& a, Vector3d& b, Vector3d& da, Vector3d& db){
    double norm = (a-b).norm();
    return (da-db)/norm - ((da-db).dot(a-b)/std::pow(norm,3)) * (a-b);
};

double EnergyFunction::normDeriv(Vector3d& a, Vector3d& b, Vector3d& da, Vector3d& db){
    double norm = (a-b).norm();
    if (norm == 0) {
        return 0;
    }
    return (a-b).dot(da-db)/norm;
};

double EnergyFunction::dxDij(double x, double xdij, Vector3d p1, Vector3d p2, Vector3d p3, Vector3d dp1, Vector3d dp2, Vector3d dp3){
    double norm1 = (p3-p2).norm();
    double norm2 = (p2-p1).norm();
    return - m_stiffnessCoef * (2/(norm1 + norm2) * xdij/std::pow(x,3) + (normDeriv(p3,p2,dp3,dp2) + normDeriv(p2,p1,dp2,dp1))/(std::pow(norm1+norm2, 2)) * (1/std::pow(x,2) - 1));
};

int EnergyFunction::correctIndex(int index){
    if (index >= m_resolution) {
        return correctIndex(index - m_resolution);
    } else if (index < 0) {
        return correctIndex(index + m_resolution);
    }
    return index;
};

double EnergyFunction::lengthFunction(double t, double t0){
    double desiredCurvature = 2;
    return 2 * M_PI * ( 1/ std::sqrt(desiredCurvature) * std::sinh(std::sqrt(desiredCurvature) * (t-t0)) + t0);
};
        