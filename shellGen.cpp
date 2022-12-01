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
    if(m_surface.size() == 0) {
        return;
    }
}

void ShellGen::expandCurveNTimes(int iterations, double length, double stiffness, double lengthCoef) {

}
    
std::vector<std::vector<Point3D>> ShellGen::getSurface() {

}