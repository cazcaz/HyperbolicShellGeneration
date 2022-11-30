#include "curveHolder.h"
#include <iostream>

CurveHolder::CurveHolder() {};

CurveHolder::~CurveHolder() {};

Curve CurveHolder::getCurve(int index) {
    int curveAmount = m_curves.size();
    if (curveAmount == 0) {
        return Curve(0);
    }
    std::cout << *m_curves[index] << std::endl;
    return *m_curves[index];
}

int CurveHolder::getSize() {
    return m_curves.size();
}

void CurveHolder::reset() {
    m_curves.clear();
}

void CurveHolder::addCurve(std::unique_ptr<Curve> newCurve) {
    m_curves.push_back(std::move(newCurve));
}