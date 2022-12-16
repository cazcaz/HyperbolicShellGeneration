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
            //Vector3d nextTangent = m_surface[curveCount-1][correctIndex(i+1)] - m_surface[curveCount-1][correctIndex(i-1)];
            nextTangent.normalize();
            Vector3d nextBinormal(normals[i][1]*nextTangent[2] - normals[i][2]*nextTangent[1] ,normals[i][2]*nextTangent[0] - normals[i][0]*nextTangent[2], normals[i][0]*nextTangent[1] - normals[i][1]*nextTangent[0]);
            //nextBinormal.normalize();
            tangents.push_back(nextTangent);
            binormals.push_back(nextBinormal);
    }

    double initialDist = 1;
    double radialDist = 1 + (curveCount-1) * m_parameters.extensionLength;
    bool success = true;

    //minimsation time
    EnergyFunction energyFunctional(m_surface[curveCount-1], normals, binormals, m_parameters, radialDist);
    LBFGSpp::LBFGSParam<double> param;
    param.max_iterations = 200;
    LBFGSpp::LBFGSSolver<double> solver(param);
    double energy;
    try {
        int iterCount = solver.minimize(energyFunctional, m_prevSol, energy);
        if (iterCount == 200) {
            m_parameters.extensionLength *= 0.5;
            std::cout << "Max iterations reached, halving extension length and trying again." << std::endl;
            success = false;
        }
    } catch(...) {
        //std::cout << "Failed from error in calcualtion." << std::endl;
        return false;
    }

    std::vector<Vector3d> nextCurve;
    for (int i =0; i<m_parameters.resolution;i++) {
        if (std::isnan(m_prevSol[i])){
            //std::cout << "Failed from nan input." << std::endl;
            return false;
        }
        nextCurve.push_back(m_surface[curveCount-1][i] + m_parameters.extensionLength * normals[i] + m_parameters.extensionLength* m_prevSol[i] * binormals[i]);
    }
    if (success) {
        m_surface.push_back(nextCurve);
    }
    return true;
}

void ShellGen::expandCurveNTimes() {
    if (m_parameters.expansions == 0) {
        int k = 0;
        //set a max to stop it getting ridiculous
        while (expandCurve() && k < 4000) {
            k++;
        }
        return;
    } else {
        for (int iteration = 0; iteration < m_parameters.expansions; iteration++){
            if (!expandCurve()){
                return;
            }
        }
    }
}

int ShellGen::correctIndex(int index){
    if (index >= m_parameters.resolution) {
        return correctIndex(index - m_parameters.resolution);
    } else if (index < 0) {
        return correctIndex(index + m_parameters.resolution);
    }
    return index;
};

void ShellGen::printSurface() {
    int fileSizeAim = 50000; //In kilobytes, assumes 100 bytes per point in mesh
    int maxRes = 200;
    int pointCount = m_parameters.resolution * m_surface.size();
    bool needsCompress = (pointCount > fileSizeAim*10.24);
    //bool needsCompress = false;
    

    ShellName namer;
    std::string fileName = namer.makeName(m_parameters);
    int surfaceLength = m_surface.size();
    if (surfaceLength < 2) {
        //not enough information to make a surface
        return; 
    }
    std::string path = "/../OutputSurfaceTxts/";
    std::ofstream open(path);
    std::ofstream surfaceFile("..\\OutputSurfaceTxts\\" + fileName + ".txt");

    if (needsCompress) {
        std::vector<int> radialIndices;
        std::vector<int> circumIndices;
        if (m_parameters.resolution > maxRes) {
            double pointGap = double(m_parameters.resolution) / double(maxRes);
            for (int i = 0; i < maxRes; i++){
                radialIndices.push_back(int(pointGap*i));
            }
            if (radialIndices[radialIndices.size()-1] != m_surface.size()-1) {
                radialIndices.push_back(m_parameters.resolution-1);
            }
            pointCount = radialIndices.size() * m_parameters.resolution;
            needsCompress = (pointCount > fileSizeAim*10.24);
        } else {
            for (int i = 0; i < maxRes; i++){
                radialIndices.push_back(i);
            }
        }
        double pointGap = double(pointCount) / (double(fileSizeAim)*10.24);
        if (needsCompress) {
            for (int i = 0; i < int(m_surface.size()/pointGap); i++){
                circumIndices.push_back(int(pointGap*i));
            }
            if (circumIndices[circumIndices.size()-1] != m_surface.size()-1) {
                circumIndices.push_back(m_surface.size()-2);
            }
        } else {
            for (int i = 0; i < m_surface.size(); i++){
                radialIndices.push_back(i);
            }
        }
        for(int circumIndex : circumIndices) {
            for(int radialIndex : radialIndices) {
            surfaceFile << m_surface[circumIndex][radialIndex][0] << ",";
            surfaceFile << m_surface[circumIndex][radialIndex][1] << ",";
            surfaceFile << m_surface[circumIndex][radialIndex][2] << " ";
            }
        surfaceFile << "\n";
        }
    } else {
        for (std::vector<Vector3d> curve : m_surface){
            for (Vector3d point : curve){
            surfaceFile << point[0] << ",";
            surfaceFile << point[1] << ",";
            surfaceFile << point[2] << " ";
            }
        surfaceFile << "\n";
        }
    }
    
    surfaceFile.close();
}


