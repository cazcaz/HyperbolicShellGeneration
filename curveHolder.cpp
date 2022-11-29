#include "curveHolder.h"

CurveHolder::CurveHolder() {};

CurveHolder::~CurveHolder() {};

Curve CurveHolder::getLastCurve() {
    int curveAmount = m_curves.size();
    if (curveAmount == 0) {
        return Curve(0);
    }
    return m_curves[curveAmount-1];
}

Curve CurveHolder::getFirstCurve() {
    int curveAmount = m_curves.size();
    if (curveAmount == 0) {
        return Curve(0);
    }
    return m_curves[0];
}

void CurveHolder::addCurve(Curve newCurve) {
    m_curves.push_back(newCurve);
}