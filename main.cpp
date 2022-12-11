#include "batchGen.h"

int main(int, char**) {
    double stiffLength = 0.00001;
    double DC = 0.00001;

    std::vector<ShellParams> parameterList;
    BatchGen massCalcer(100);

    for (int i=0; i<5;i++){
        stiffLength = 0.00001;
        DC *= 10;
        for (int j=0; j<10;j++){
            stiffLength *= 10;
            ShellParams parameters;
            parameters.resolution = 500;
            parameters.extensionLength = 0.1;
            parameters.stiffLengthRatio = 0.00001;
            parameters.desiredCurvature = DC;
            parameters.stiffLengthRatio = stiffLength;
            parameterList.push_back(parameters);
        }
    }
    massCalcer.calculateAll(parameterList);
    return 0;
}
