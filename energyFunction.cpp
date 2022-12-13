#include "energyFunction.h"
#include <cmath>
#include <iostream>

using Eigen::Vector3d;
using Eigen::VectorXd;

EnergyFunction::EnergyFunction(std::vector<Vector3d>& currentCurve,
                               std::vector<Vector3d>& normals,
                               std::vector<Vector3d>& binormals,
                               ShellParams& parameters,
                               double radialDist) : 
                               m_currentCurve(currentCurve) ,
                               m_normals(normals) ,
                               m_binormals(binormals) ,
                               m_parameters(parameters) ,
                               m_radialDist(radialDist) {
};

EnergyFunction::~EnergyFunction() = default;

double EnergyFunction::operator()(const VectorXd& inputs, VectorXd& derivatives){
    std::vector<Vector3d> nextCurve;
    std::vector<Vector3d> prevCurve;
    double scalingTerm = rescaleEnergyFunction(m_radialDist, 1);
    for (int i=0; i<m_parameters.resolution; i++) {
        nextCurve.push_back(m_currentCurve[i] + m_parameters.extensionLength*m_normals[i] + m_parameters.extensionLength* inputs[i]*m_binormals[i]);
        prevCurve.push_back(m_currentCurve[i] - m_parameters.extensionLength*m_normals[i]);
    }
    double circumferentialEnergySum = 0;
    double totalLength = 0;
    double radialEnergySum = 0;

    //Find the length of the curve before the main loop so it can be contained in a single loop
    for (int i=0; i<m_parameters.resolution;i++){
        Vector3d lastPoint = nextCurve[correctIndex(i-1)];
        Vector3d currentPoint = nextCurve[correctIndex(i)];
        totalLength += (currentPoint-lastPoint).norm();
    }

    //Evaluate energy and derivative in the same loop to save time
    for (int i=0; i<m_parameters.resolution;i++){
        //Points on the current curve used in each calculation
        Vector3d p1 = nextCurve[correctIndex(i-2)];
        Vector3d p2 = nextCurve[correctIndex(i-1)];
        Vector3d p3 = nextCurve[correctIndex(i)];
        Vector3d p4 = nextCurve[correctIndex(i+1)];
        Vector3d p5 = nextCurve[correctIndex(i+2)];
        Vector3d prevPoint = prevCurve[correctIndex(i)];
        Vector3d currentPoint = m_currentCurve[correctIndex(i)];

        //Derivatives of each point w.r.t. ith input
        Vector3d dp1 = Vector3d::Zero(3);
        Vector3d dp2 = Vector3d::Zero(3);
        Vector3d dp3 = m_binormals[i];
        Vector3d dp4 = Vector3d::Zero(3);
        Vector3d dp5 = Vector3d::Zero(3);
        Vector3d dpP = Vector3d::Zero(3);
        Vector3d dcP = Vector3d::Zero(3);

        //Energy calcs. 
        circumferentialEnergySum += scalingTerm*bendingEnergy(p2,p3,p4);
        radialEnergySum += scalingTerm*bendingEnergy(prevPoint, currentPoint, p3);

        
        double energyBendDeriv = scalingTerm*m_parameters.stiffLengthRatioCircum *(bendingEnergyDeriv(p3,p4,p5,dp3,dp4,dp5) + bendingEnergyDeriv(p2,p3,p4,dp2,dp3,dp4) + bendingEnergyDeriv(p1,p2,p3,dp1,dp2,dp3));
        double energyRadialBendDeriv = scalingTerm*m_parameters.stiffLengthRatioRadial*(bendingEnergyDeriv(prevPoint, currentPoint, p3, dpP, dcP, dp3));
        double lengthEnergyDeriv = scalingTerm*(2 * (totalLength - lengthFunction(m_radialDist, 1)) * ((dp4-dp3).dot((p4-p3).normalized()) + (dp3-dp2).dot((p3-p2).normalized())));
        derivatives[i] = energyBendDeriv + energyRadialBendDeriv + lengthEnergyDeriv;
        
        //Isolated energies for testing
        //derivatives[i] = energyBendDeriv;
        //derivatives[i] = energyRadialBendDeriv;
        //derivatives[i] = lengthEnergyDeriv;
    }

    double lengthEnergy = scalingTerm * std::pow(totalLength-lengthFunction(m_radialDist, 1),2);
    double totalEnergy = m_parameters.stiffLengthRatioCircum * circumferentialEnergySum + m_parameters.stiffLengthRatioRadial * radialEnergySum + lengthEnergy;
    
    //Isolated energy derivatives for testing
    //std::cout << std::fixed << "Bending Energy: " <<  m_parameters.stiffLengthRatio * circumferentialEnergySum << "  Radial Bending Energy: " << m_parameters.stiffLengthRatio *  radialEnergySum << "  Length Energy: " << lengthEnergy << "  Total Energy: " << totalEnergy << std::endl;

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
    return 2/(norm1 + norm2) * (xdij/std::pow(x-1,2))  - (normDeriv(p3,p2,dp3,dp2) + normDeriv(p2,p1,dp2,dp1))/(std::pow(norm1+norm2, 2)) * std::pow(std::tan((M_PI - std::acos(x))/2),2);
};

int EnergyFunction::correctIndex(int index){
    if (index >= m_parameters.resolution) {
        return correctIndex(index - m_parameters.resolution);
    } else if (index < 0) {
        return correctIndex(index + m_parameters.resolution);
    }
    return index;
};

double EnergyFunction::lengthFunction(double t, double t0){
    double sqrtDC = std::sqrt(m_parameters.desiredCurvature);
    return 2 * M_PI * ( 1/sqrtDC * std::sinh(sqrtDC * (t-t0)) + t0);
};
    
double EnergyFunction::rescaleEnergyFunction(double t, double t0){
    return 1/(std::pow(m_parameters.resolution,2) *lengthFunction(t,t0));
};

double EnergyFunction::heavisideApprox(double t){
    return 1/(1+ std::exp(-4*t));
};

double EnergyFunction::inverseLengthFunction(double t, double t0){
    double sqrtDC = std::sqrt(m_parameters.desiredCurvature);
    return t0 + std::asinh(sqrtDC * t/M_2_PI)/sqrtDC;
};

double EnergyFunction::bendingEnergy(Vector3d a, Vector3d b, Vector3d c){
    double cosAngle = (c-b).normalized().dot((a-b).normalized());
    return (1/((c-b).norm() + (a-b).norm()) * std::pow(std::tan((M_PI - std::acos(cosAngle))/2),2));
};

double EnergyFunction::bendingEnergyDeriv(Vector3d a, Vector3d b, Vector3d c, Vector3d da, Vector3d db, Vector3d dc)
{
    double dxdij = ((c-b).normalized().dot(normalVecDeriv(a,b,da,db)) + (a-b).normalized().dot(normalVecDeriv(c,b,dc,db)));
    double cosAngle = (c-b).normalized().dot((a-b).normalized());
    return dxDij(cosAngle, dxdij, a, b, c, da, db, dc);
};
