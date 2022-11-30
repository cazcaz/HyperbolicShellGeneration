#pragma once
#include "curveHolder.h"
#include "curve.h"
#include <memory>

class ShellGen {
    public:
        ShellGen();
        ~ShellGen();

        void setInitCurve(Curve initialCurve);

        void expandCurve(double length, double stiffness = 0.05, double lengthCoef = 100);

        void expandCurveNTimes(int iterations, double length, double stiffness = 0.05, double lengthCoef = 100);
    
        CurveHolder getSurface();
    private:
        CurveHolder m_surface;
        int m_resolution;
};