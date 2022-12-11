#include "batchGen.h"

int main(int, char**) {
    double stiffLengthStart = 0.001;
    double DCStart = 0.0001;
    double stiffLengthEnd = 10000;
    double DCEnd = 0.01;
    int parameterSpaceResolution = 49;

    std::vector<ShellParams> parameterList;
    BatchGen massCalcer;

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
            parameters.extensionLength = 0.5;
            parameters.expansions = 2000;
            parameters.desiredCurvature = DC;
            parameters.stiffLengthRatio = stiff;
            parameterList.push_back(parameters);
        }
    }

    massCalcer.calculateAll(parameterList);
    return 0;
}
