#include "shellGen.h"
#include "shellParams.h"

int main(int, char**) {
    
    ShellParams parameters;
    parameters.extensionLength = 0.01;
    parameters.lengthStiffnessCoef = 100000;

    ShellGen shellGenerator(parameters);
    shellGenerator.setInitCurve();
    shellGenerator.expandCurveNTimes(1000);
    shellGenerator.printSurface();
    return 0;
}
