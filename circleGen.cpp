#pragma once
#include "circleGen.h"
#include <cmath>
#include <iostream>

CircleGen::CircleGen() {
    
};

CircleGen::~CircleGen() {};

std::unique_ptr<Curve> CircleGen::makeCircle(double radius, Point3D centre, int resolution) {
    std::unique_ptr<Curve> circle = std::make_unique<Curve>(resolution);
    for (int i = 0;  i < resolution; i++){
        float angle = 2 * M_PI * i / resolution;
        Point3D next(radius * std::cos(angle), radius * std::sin(angle), 0);
        circle->setValue(i, centre + next);
    }
    return std::move(circle);
}