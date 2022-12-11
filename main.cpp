#include "batchGen.h"

int main(int, char**) {
    double stiffLengthStart = 0.00001;
    double DCStart = 0.00001;
    double stiffLengthEnd = 0.01;
    double DCEnd = 0.01;
    int parameterSpaceResolution = 49;

    std::vector<ShellParams> parameterList;
    BatchGen massCalcer(2000);

    std::vector<double> stiffLengths;
    std::vector<double> DCs;
    for (int i=0; i<=parameterSpaceResolution; i++){
        stiffLengths.push_back((stiffLengthEnd-stiffLengthStart)/parameterSpaceResolution * i + stiffLengthStart);
        DCs.push_back((DCEnd-DCStart)/parameterSpaceResolution * i + DCStart);
    }

    for (double stiff : stiffLengths){
        for (double DC : DCs) {
            ShellParams parameters;
            parameters.resolution = 500;
            parameters.extensionLength = 0.1;
            parameters.stiffLengthRatio = 0.00001;
            parameters.desiredCurvature = DC;
            parameters.stiffLengthRatio = stiff;
            parameterList.push_back(parameters);
        }
    }

    massCalcer.calculateAll(parameterList);
    return 0;
}
