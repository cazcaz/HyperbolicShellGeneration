#pragma once
#include <vector>
#include "curve.h"
#include <memory>

class CurveHolder {
    public:
        CurveHolder();
        ~CurveHolder();
    
        Curve getCurve(int index);

        void addCurve(std::unique_ptr<Curve> newCurve);
        int getSize();
        void reset();
    private:
        std::vector<std::unique_ptr<Curve>> m_curves;
};