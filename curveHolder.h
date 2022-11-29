#pragma once
#include <vector>
#include "curve.h"

class CurveHolder {
    public:
        CurveHolder();
        ~CurveHolder();
    
        Curve getLastCurve();
        Curve getFirstCurve();

        void addCurve(Curve newCurve);
    private:
        std::vector<Curve> m_curves;
};