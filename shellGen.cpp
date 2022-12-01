#include "shellGen.h"
#include "vectorMaths.h"
#include "circleGen.h"
#include <cmath>
#include <iostream>

ShellGen::ShellGen() : m_resolution(0) {};
ShellGen::~ShellGen() {};

void ShellGen::setInitCurve(double radius, Point3D centre, int resolution) {

    m_resolution = resolution;
    m_surface.clear();
    std::vector<Point3D> initCurve;
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
    std::vector<Point3D> normals;
    std::vector<Point3D> binormals;
    std::vector<Point3D> tangents;
    if (curveCount == 1) {
        for (Point3D firstCurvePoint : m_surface[0]){
            normals.push_back(firstCurvePoint);
        }
    } else {
        for (int i =0; i<m_resolution;i++){
            Point3D nextPoint = m_surface[curveCount-1][i] - m_surface[curveCount-2][i];
            vMathsHandler.normalise(&nextPoint);
            normals.push_back(nextPoint);
        }
    }
    //Nicely behaved tangents
    double angleChange = 2 * M_PI / m_resolution;
    for (int i =0; i<m_resolution;i++){
            Point3D nextTangent(-std::sin(angleChange * i), std::cos(angleChange *i), 0);
            vMathsHandler.normalise(&nextTangent);
            Point3D nextBinormal = vMathsHandler.cross(normals[i], nextTangent);
            vMathsHandler.normalise(&nextBinormal);
            tangents.push_back(nextTangent);
            binormals.push_back(nextBinormal);
    }
    
}

void ShellGen::expandCurveNTimes(int iterations, double length, double stiffness, double lengthCoef) {

}
    
std::vector<std::vector<Point3D>> ShellGen::getSurface() {

}