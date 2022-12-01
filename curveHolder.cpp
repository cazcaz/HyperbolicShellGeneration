#include "curveHolder.h"
#include <iostream>

CurveHolder::CurveHolder() {};

CurveHolder::~CurveHolder() {};

std::shared_ptr<Curve> CurveHolder::getCurve(int index) {
    if (m_curves.empty()) {
        return std::move(std::make_unique<Curve>(0));
    }
    std::shared_ptr<Curve> curveCopy = std::move(m_curves[index]);
    return curveCopy;
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