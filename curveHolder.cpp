#include "curveHolder.h"
#include <iostream>

CurveHolder::CurveHolder() {};

CurveHolder::~CurveHolder() {};

Curve CurveHolder::getCurve(int index) {
    int curveAmount = m_curves.size();
    if (curveAmount == 0) {
        return Curve(0);
    }
    Curve returnCurve(*m_curves[index]);
    std::cout << returnCurve << std::endl;
    return returnCurve;
}

int CurveHolder::getSize() {
    return m_curves.size();
}

void CurveHolder::reset() {
    m_curves.clear();
}

void CurveHolder::addCurve(Curve* newCurve) {
    m_curves.push_back(newCurve);
}