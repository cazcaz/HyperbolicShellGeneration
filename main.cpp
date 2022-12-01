#include <iostream>
#include "circleGen.h"
#include "shellGen.h"

int main(int, char**) {
    ShellGen shellGenerator;
    shellGenerator.setInitCurve(1, Point3D(0,0,0), 5);
    shellGenerator.expandCurve(1, 0.05, 100);
    return 0;
}
