#pragma once
#include "circleGen.h"
#include <cmath>
#include <iostream>

CircleGen::CircleGen() {
    
};

CircleGen::~CircleGen() {};

void CircleGen::makeCircle(double radius, Point3D centre, int resolution, std::vector<Point3D> &curve) {
    for (int i = 0;  i < resolution; i++){
        float angle = 2 * M_PI * i / resolution;
        Point3D next(radius * std::cos(angle), radius * std::sin(angle), 0);
        curve.push_back(centre + next);
    }
}