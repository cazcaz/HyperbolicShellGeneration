#pragma once
#include <vector>
#include "curve.h"
#include <memory>

class CurveHolder {
    public:
        CurveHolder();
        ~CurveHolder();
    
        std::shared_ptr<Curve> getCurve(int index);

        friend std::ostream& operator<<(std::ostream& os, CurveHolder const &curveHolder){
            int curveCount = curveHolder.m_curves.size();
            os << "{";
            if (curveCount > 1){
                for (int i=0; i< curveCount - 1; i++){
                os << *curveHolder.m_curves[i] << ", ";
                };
            } else if (curveCount == 1) {
                os << *curveHolder.m_curves[0];
            }
            os << *curveHolder.m_curves[curveCount - 1] << "}";
            return os;
        };

        void addCurve(std::unique_ptr<Curve> newCurve);
        int getSize();
        void reset();
    private:
        std::vector<std::unique_ptr<Curve>> m_curves;
};