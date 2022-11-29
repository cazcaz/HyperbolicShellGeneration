#pragma once
#include "circleGen.h"
#include <cmath>
#include <iostream>

CircleGen::CircleGen() {
    
};

CircleGen::~CircleGen() {};

Curve CircleGen::makeCircle(float radius, Point3D centre, int resolution) {
    Curve circle(resolution);
    for (int i = 0;  i < resolution; i++){
        float angle = 2 * M_PI * i / resolution;
        Point3D next(radius * std::cos(angle), radius * std::sin(angle), 0);
        circle.setValue(i, centre + next);
    }
    return circle;
}