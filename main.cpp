#include <iostream>
#include "circleGen.h"
#include "shellGen.h"
#include "curveHolder.h"

int main(int, char**) {
    CircleGen circleGenerator;
    std::unique_ptr<Curve> circle = circleGenerator.makeCircle(1, Point3D(0,0,0), 5);
    ShellGen shellGenerator;
    shellGenerator.setInitCurve(std::move(circle));
    shellGenerator.expandCurve(1, 0.05, 100);
    std::cout << "success" << std::endl;
    return 0;
}
