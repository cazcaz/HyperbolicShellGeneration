#pragma once
#include "curve.h"

Curve::Curve(int resolution) : m_resolution(resolution) , m_points() {
    m_points.reserve(m_resolution);
};

Curve::~Curve(){};

void Curve::setValue(int index, Point3D point){
    m_points[resolveIndex(index)] = point;
};

Point3D Curve::getValue(int index){
    return m_points[resolveIndex(index)];
};

int Curve::resolveIndex(int index){
    if (index > m_resolution - 1) {
        return resolveIndex(index - m_resolution);
    } else if (index < 0) {
        return resolveIndex(index + m_resolution);
    }
    return index;
}
