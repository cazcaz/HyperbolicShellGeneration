#include "shellGen.h"
#include "shellParams.h"

int main(int, char**) {
    
    ShellParams parameters;
    parameters.resolution = 100;
    parameters.extensionLength = 0.01;
    parameters.stiffLengthRatio = 1;
    parameters.desiredCurvature = 0.001;
    ShellGen shellGenerator(parameters);

    for (int j = 1; j <= 10; j++){
        parameters.desiredCurvature *= 10;
        for (int i = 1; i <= 10; i++){
            parameters.stiffLengthRatio *= 10;
            shellGenerator.setInitCurve();
            shellGenerator.expandCurveNTimes(100);
            shellGenerator.printSurface();
        }
    }
    return 0;
}
