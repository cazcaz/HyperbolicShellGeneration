#include <iostream>
#include "circleGen.h"
#include "shellGen.h"

int main(int, char**) {
    CircleGen circleGenerator;
    Curve circle = circleGenerator.makeCircle(1, Point3D(0,0,0), 5);
    ShellGen shellGenerator;
    shellGenerator.setInitCurve(&circle);
    shellGenerator.expandCurve(1, 0.05, 100);
    return 0;
}
