#pragma once
#include <vector>
#include "curve.h"
#include <memory>

class CurveHolder {
    public:
        CurveHolder();
        ~CurveHolder();
    
        void getCurve(int index, Curve* curve);

        void addCurve(Curve newCurve);
        int getSize();
        void reset();
    private:
        std::vector<Curve> m_curves;
};