#include "shellGen.h"
#include "shellParams.h"

int main(int, char**) {
    
    ShellParams parameters;
    parameters.extensionLength = 0.5;
    parameters.stiffLengthRatio = 1000;
    parameters.desiredCurvature = 0.00001;

    ShellGen shellGenerator(parameters);
    shellGenerator.setInitCurve();
    shellGenerator.expandCurveNTimes(100);
    shellGenerator.printSurface();
    return 0;
}
