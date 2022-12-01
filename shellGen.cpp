#include "shellGen.h"
#include "vectorMaths.h"
#include <cmath>
#include <iostream>

ShellGen::ShellGen() : m_resolution(0) {};
ShellGen::~ShellGen() {};

void ShellGen::setInitCurve(Curve initialCurve) {
    m_surface.reset();
    m_resolution = initialCurve.getResolution();
    m_surface.addCurve(initialCurve);
}

void ShellGen::expandCurve(double length, double stiffness, double lengthCoef) {
    VectorMaths vMathsHandler;
    int curveAmount = m_surface.getSize();
    if (curveAmount == 0) {
        return;
    }
    Curve prevCurve(m_resolution);
    m_surface.getCurve(curveAmount-1, &prevCurve);
    Curve tangents(m_resolution);
    Curve normals(m_resolution);
    Curve binormals(m_resolution);
    if (curveAmount == 1) {
        Curve first(m_resolution);
        m_surface.getCurve(0, &first);
        Point3D nextPoint(0,0,0);
        for (int i=0; i<m_resolution;i++){
            first.getValue(i, &nextPoint);
            vMathsHandler.normalise(&nextPoint);
            normals.setValue(i, nextPoint);
        }
    } else {
        std::cout << "here" <<std::endl;
        Curve secondLast(m_resolution);
        m_surface.getCurve(m_resolution-2, &secondLast);
        Point3D secondLastPoint(0,0,0);
        Point3D lastPoint(0,0,0);
        for (int i=0; i<m_resolution;i++){
            secondLast.getValue(i, &secondLastPoint);
            prevCurve.getValue(i, &lastPoint);
            Point3D nextPoint = lastPoint - secondLastPoint;
            vMathsHandler.normalise(&nextPoint);
            normals.setValue(i, nextPoint);
        }
    }
    //Tangents is assumed to be the nice behaving straight one, not defined by the previous curves
    float angleChange = 2 * M_PI / m_resolution;
    Point3D normalHold(0,0,0);
    for (int i=0; i<m_resolution;i++){
            float angle =  i * angleChange;
            Point3D nextPoint(-std::sin(angle), std::cos(angle) ,0);
            vMathsHandler.normalise(&nextPoint);
            tangents.setValue(i, nextPoint);
            normals.getValue(i, &normalHold);
            Point3D binormal = vMathsHandler.cross(normalHold, nextPoint);
            vMathsHandler.normalise(&binormal);
            binormals.setValue(i, binormal);
    }
    std::cout<<"here"<<std::endl;
    std::cout << normals << tangents << binormals << std::endl;
}

void ShellGen::expandCurveNTimes(int iterations, double length, double stiffness, double lengthCoef) {

}
    
CurveHolder ShellGen::getSurface() {

}