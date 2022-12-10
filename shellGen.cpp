#include "shellGen.h"
#include "circleGen.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include "LBFGS.h"
#include "energyFunction.h"

using Eigen::Vector3d;

ShellGen::ShellGen(ShellParams& parameters) : m_parameters(parameters) , m_prevSol(0.05 * VectorXd::Random(parameters.resolution)) {};
ShellGen::~ShellGen() {};

void ShellGen::setInitCurve() {
    Vector3d centre(m_parameters.centreX, m_parameters.centreY, m_parameters.centreZ);
    m_surface.clear();
    std::vector<Vector3d> initCurve;
    CircleGen circlemaker;
    circlemaker.makeCircle(1, centre, m_parameters.resolution, initCurve);
    m_surface.push_back(initCurve);
}

bool ShellGen::expandCurve() {
    int curveCount = m_surface.size();
    if(curveCount == 0) {
        return false;
    }
    std::vector<Vector3d> normals;
    std::vector<Vector3d> binormals;
    std::vector<Vector3d> tangents;
    if (curveCount == 1) {
        for (Vector3d firstCurvePoint : m_surface[0]){
            normals.push_back(firstCurvePoint);
        }
    } else {
        for (int i =0; i<m_parameters.resolution;i++){
            Vector3d nextPoint = m_surface[curveCount-1][i] - m_surface[curveCount-2][i];
            nextPoint.normalize();
            normals.push_back(nextPoint);
        }
    }
    //Nicely behaved tangents
    double angleChange = 2 * M_PI / m_parameters.resolution;
    for (int i =0; i<m_parameters.resolution;i++){
            Vector3d nextTangent(-std::sin(angleChange * i), std::cos(angleChange *i), 0);
            nextTangent.normalize();
            Vector3d nextBinormal(normals[i][1]*nextTangent[2] - normals[i][2]*nextTangent[1] ,normals[i][2]*nextTangent[0] - normals[i][0]*nextTangent[2], normals[i][0]*nextTangent[1] - normals[i][1]*nextTangent[0]);
            //nextBinormal.normalize();
            tangents.push_back(nextTangent);
            binormals.push_back(nextBinormal);
    }

    double initialDist = 1;
    double radialDist = 1 + (curveCount-1) * m_parameters.extensionLength;

    //minimsation time
    EnergyFunction energyFunctional(m_surface[curveCount-1], normals, binormals, m_parameters, radialDist);
    LBFGSpp::LBFGSParam<double> param;
    param.max_iterations = 100;
    LBFGSpp::LBFGSSolver<double> solver(param);
    double energy;
    try {
        int iterCount = solver.minimize(energyFunctional, m_prevSol, energy);
        if (iterCount == 100) {
            std::cout << "Failed from max iterations." << std::endl;
            return false;
        }
    } catch(...) {
        std::cout << "Failed from error in calcualtion." << std::endl;
        return false;
    }

    std::vector<Vector3d> nextCurve;
    for (int i =0; i<m_parameters.resolution;i++) {
        if (std::isnan(m_prevSol[i])){
            std::cout << "Failed from nan input." << std::endl;
            return false;
        }
        nextCurve.push_back(m_surface[curveCount-1][i] + m_parameters.extensionLength * normals[i] + m_prevSol[i] * binormals[i]);
    }
    m_surface.push_back(nextCurve);
    return true;
}

void ShellGen::expandCurveNTimes(int iterations) {
    if (iterations == 0) {
        int k = 0;
        //set a max to stop it getting ridiculous
        while (expandCurve() && k < 4000) {
            k++;
        }
        std::cout << k << " curves found before failure." <<std::endl;
        return;
    } else {
        for (int iteration = 0; iteration < iterations; iteration++){
            if (!expandCurve()){
                std::cout << "Failed on curve " << iteration + 1 << std::endl;
                return;
            }
        }
    }
    std::cout << "Success." << std::endl;
}

void ShellGen::printSurface() {
    ShellName namer;
    std::string fileName = namer.makeName(m_parameters); 
    std::cout << fileName << std::endl;
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


