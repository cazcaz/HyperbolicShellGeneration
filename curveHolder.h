#pragma once
#include <vector>
#include "curve.h"
#include <memory>

class CurveHolder {
    public:
        CurveHolder();
        ~CurveHolder();
    
        Curve getCurve(int index);

        friend std::ostream& operator<<(std::ostream& os, CurveHolder const &curveHolder){
            int curveCount = curveHolder.m_curves.size();
            os << "{";
            for (int i=0; i< curveCount - 1; i++){
                os << curveHolder.m_curves[i] << ", ";
            };
            os << curveHolder.m_curves[curveCount - 1] << "}";
            return os;
        };

        void addCurve(Curve* newCurve);
        int getSize();
        void reset();
    private:
        std::vector<Curve*> m_curves;
};