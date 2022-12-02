#include <iostream>
#include "shellGen.h"

int main(int, char**) {
    ShellGen shellGenerator;
    shellGenerator.setInitCurve(1, 0,0,0, 5);
    //shellGenerator.expandCurve(1, 0.05, 100);
    shellGenerator.expandCurveNTimes(100 , 0.01, 1000, 100);
    shellGenerator.printSurface("FirstSurface");
    return 0;
}
