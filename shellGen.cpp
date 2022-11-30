#include "shellGen.h"
#include "vectorMaths.h"
#include <cmath>
#include <iostream>

ShellGen::ShellGen() : m_resolution(0) , m_surface() {};
ShellGen::~ShellGen() {};

void ShellGen::setInitCurve(Curve* initialCurve) {
    m_surface.reset();
    m_resolution = initialCurve->getResolution();
    m_surface.addCurve(initialCurve);
}

void ShellGen::expandCurve(double length, double stiffness, double lengthCoef) {
    VectorMaths vMathsHandler;
    std::cout << m_surface << std::endl;
    int curveAmount = m_surface.getSize();
    if (curveAmount == 0) {
        return;
    }
    Curve prevCurve = m_surface.getCurve(curveAmount-1);
    std::cout << prevCurve << std::endl;
    Curve tangents(m_resolution);
    Curve normals(m_resolution);
    Curve binormals(m_resolution);
    if (curveAmount == 1) {
        Curve first = m_surface.getCurve(0);
        for (int i=0; i<m_resolution;i++){
            Point3D nextPoint = first.getValue(i);
            vMathsHandler.normalise(&nextPoint);
            normals.setValue(i, nextPoint);
        }
    } else {
        Curve secondLast = m_surface.getCurve(m_resolution-2);
        Curve last = m_surface.getCurve(m_resolution-1);
        for (int i=0; i<m_resolution;i++){
            Point3D nextPoint = last.getValue(i) - secondLast.getValue(i);
            vMathsHandler.normalise(&nextPoint);
            normals.setValue(i, nextPoint);
        }
    }
    //Tangents is assumed to be the nice behaving straight one, not defined by the previous curves
    float angleChange = 2 * M_PI / m_resolution;
    for (int i=0; i<m_resolution;i++){
            float angle =  i * angleChange;
            Point3D nextPoint(-std::sin(angle), std::cos(angle) ,0);
            vMathsHandler.normalise(&nextPoint);
            tangents.setValue(i, nextPoint);
            Point3D binormal = vMathsHandler.cross(normals.getValue(i), nextPoint);
            std::cout << binormal << std::endl;
            vMathsHandler.normalise(&binormal);
            std::cout << binormal << std::endl;
            binormals.setValue(i, binormal);
    }
    //std::cout << normals << std::endl;
    //std::cout << tangents << std::endl;
    //std::cout << binormals << std::endl;
}

void ShellGen::expandCurveNTimes(int iterations, double length, double stiffness, double lengthCoef) {

}
    
CurveHolder ShellGen::getSurface() {

}