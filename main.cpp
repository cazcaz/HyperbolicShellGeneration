#include "shellGen.h"
#include "shellParams.h"

int main(int, char**) {
    
    ShellParams parameters;
    parameters.initRadius = 20;
    parameters.extensionLength = 0.5;
    parameters.stiffLengthRatio = 1000;
    parameters.desiredCurvature = 0.00001;

    ShellGen shellGenerator(parameters);
    shellGenerator.setInitCurve();
    shellGenerator.expandCurveNTimes(1000);
    shellGenerator.printSurface();
    return 0;
}
