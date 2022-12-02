#include "shellGen.h"
#include "circleGen.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "LBFGS.h"
#include "energyFunction.h"

using Eigen::Vector3d;

ShellGen::ShellGen() : m_resolution(0) {};
ShellGen::~ShellGen() {};

void ShellGen::setInitCurve(double radius, double centreX, double centreY, double centreZ, int resolution) {
    m_resolution = resolution;
    Vector3d centre(centreX, centreY, centreZ);
    m_surface.clear();
    std::vector<Vector3d> initCurve;
    CircleGen circlemaker;
    circlemaker.makeCircle(radius, centre, resolution, initCurve);
    m_surface.push_back(initCurve);
}

void ShellGen::expandCurve(double length, double stiffness, double lengthCoef) {
    int curveCount = m_surface.size();
    if(curveCount == 0) {
        return;
    }
    std::vector<Vector3d> normals;
    std::vector<Vector3d> binormals;
    std::vector<Vector3d> tangents;
    if (curveCount == 1) {
        for (Vector3d firstCurvePoint : m_surface[0]){
            normals.push_back(firstCurvePoint);
        }
    } else {
        for (int i =0; i<m_resolution;i++){
            Vector3d nextPoint = m_surface[curveCount-1][i] - m_surface[curveCount-2][i];
            nextPoint.normalize();
            normals.push_back(nextPoint);
        }
    }
    //Nicely behaved tangents
    double angleChange = 2 * M_PI / m_resolution;
    for (int i =0; i<m_resolution;i++){
            Vector3d nextTangent(-std::sin(angleChange * i), std::cos(angleChange *i), 0);
            nextTangent.normalize();
            Vector3d nextBinormal(normals[i][1]*nextTangent[2] - normals[i][2]*nextTangent[1] ,normals[i][2]*nextTangent[0] - normals[i][0]*nextTangent[2], normals[i][0]*nextTangent[1] - normals[i][1]*nextTangent[0]);
            //nextBinormal.normalize();
            tangents.push_back(nextTangent);
            binormals.push_back(nextBinormal);
    }

    double initialDist = m_surface[0][0].norm();
    double radialDist = initialDist + (curveCount-1) * length;

    //minimsation time
    EnergyFunction energyFunctional(m_surface[curveCount-1], normals, binormals, length, stiffness, lengthCoef, radialDist, initialDist);
    LBFGSpp::LBFGSParam<double> param;
    LBFGSpp::LBFGSSolver<double> solver(param);
    VectorXd randoms = VectorXd::Random(m_resolution);
    
    double rangeScale = 0.05;
    VectorXd inputs =  randoms * rangeScale;
    double energy;
    double iterCount = solver.minimize(energyFunctional, inputs, energy);
    std::vector<Vector3d> nextCurve;
    for (int i =0; i<m_resolution;i++) {
        nextCurve.push_back(m_surface[curveCount-1][i] + length * normals[i] + inputs[i] * binormals[i]);
    }
    m_surface.push_back(nextCurve);
}

void ShellGen::expandCurveNTimes(int iterations, double length, double stiffness, double lengthCoef) {
    for (int iteration = 0; iteration < iterations; iteration++){
        std::cout << iteration << std::endl;
        expandCurve(length, stiffness, lengthCoef);
    }
}

void ShellGen::printSurface(std::string fileName) {
    int surfaceLength = m_surface.size();
    if (surfaceLength < 2) {
        //not enough information to make a surface
        return; 
    }
    std::string path = "/../OutputSurfaceTxts/";
    std::ofstream open(path);
    std::ofstream surfaceFile("..\\OutputSurfaceTxts\\" + fileName + ".txt");

    int endCheck = 0;
    for (std::vector<Vector3d> curve : m_surface){
        endCheck++;
        for (Vector3d point : curve){
            surfaceFile << point[0] << ",";
            surfaceFile << point[1] << ",";
            surfaceFile << point[2] << " ";
        }
        surfaceFile << "\n";
    }
    surfaceFile.close();
}


