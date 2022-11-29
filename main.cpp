#include <iostream>
#include "circleGen.h"

int main(int, char**) {
    CircleGen circleGenerator;
    Curve circle = circleGenerator.makeCircle(1, Point3D(0,0,0), 5);
    std::cout << circle << std::endl;
    return 0;
}
