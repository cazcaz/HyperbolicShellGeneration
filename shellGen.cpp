#include "shellGen.h"
#include "vectorMaths.h"
#include "circleGen.h"
#include <cmath>
#include <iostream>
#include "LBFGS.h"

using Eigen::Vector3d;

ShellGen::ShellGen() : m_resolution(0) {};
ShellGen::~ShellGen() {};

void ShellGen::setInitCurve(double radius, Vector3d centre, int resolution) {
    m_resolution = resolution;
    m_surface.clear();
    std::vector<Vector3d> initCurve;
    CircleGen circlemaker;
    circlemaker.makeCircle(radius, centre, resolution, initCurve);
    m_surface.push_back(initCurve);
}

void ShellGen::expandCurve(double length, double stiffness, double lengthCoef) {
    VectorMaths vMathsHandler;
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
    for (Vector3d point : binormals) {
        std::cout << point << std::endl;

    }
}

void ShellGen::expandCurveNTimes(int iterations, double length, double stiffness, double lengthCoef) {

}