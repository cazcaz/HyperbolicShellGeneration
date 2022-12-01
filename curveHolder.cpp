#include "curveHolder.h"
#include <iostream>
#include <memory>

CurveHolder::CurveHolder() {};

CurveHolder::~CurveHolder() {};

void CurveHolder::getCurve(int index, Curve* curve) {
    int curveAmount = m_curves.size();
    if (curveAmount == 0) {
        *curve = Curve(0);
    }
   *curve = m_curves[index];
}

int CurveHolder::getSize() {
    return m_curves.size();
}

void CurveHolder::reset() {
    m_curves.clear();
}

void CurveHolder::addCurve(Curve newCurve) {
    m_curves.push_back(newCurve);
}