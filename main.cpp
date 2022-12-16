#include "batchGen.h"

int main(int, char**) {
    double stiffLengthStart = 0.01;
    double DCStart = 0.0001;
    double stiffLengthEnd = 0.1;
    double DCEnd = 0.01;
    double ExLStart = 0.1;
    double ExLEnd = 10;
    int parameterSpaceResolution = 1;

    std::vector<ShellParams> parameterList;
    BatchGen massCalcer;

    std::vector<double> stiffLengthsCircum;
    std::vector<double> stiffLengthsRadial;
    std::vector<double> DCs;
    std::vector<double> ExLs;
    for (int i=0; i<=parameterSpaceResolution; i++){
        stiffLengthsCircum.push_back((stiffLengthEnd-stiffLengthStart)/parameterSpaceResolution * i + stiffLengthStart);
        stiffLengthsRadial.push_back((stiffLengthEnd-stiffLengthStart)/parameterSpaceResolution * i + stiffLengthStart);
        DCs.push_back((DCEnd-DCStart)/parameterSpaceResolution * i + DCStart);
        ExLs.push_back((ExLEnd-ExLStart)/parameterSpaceResolution * i + ExLStart);

    }
    
    // for (double stiff : stiffLengths){
    //     for (double DC : DCs) {
    //         ShellParams parameters;
    //         parameters.resolution = 500;
    //         parameters.extensionLength = 0.1;
    //         parameters.expansions = 500;
    //         parameters.desiredCurvature = DC;
    //         parameters.stiffLengthRatio = stiff;
    //         parameterList.push_back(parameters);
    //     }
    // }

        // for (double stiffLengthCirc : stiffLengthsCircum) {
        //     for (double stiffLengthRad : stiffLengthsRadial) {
        //     ShellParams parameters;
        //     parameters.resolution = 10;
        //     parameters.extensionLength = 0.1;
        //     parameters.expansions = 9;
        //     parameters.desiredCurvature = 0.001;
        //     parameters.stiffLengthRatioCircum = stiffLengthCirc;
        //     parameters.stiffLengthRatioRadial = stiffLengthRad;
        //     parameterList.push_back(parameters);
        //     }
        // }

        int orig = 1000;
            ShellParams parameters;
            parameters.resolution = 5000;
            parameters.extensionLength = 10;
            parameters.expansions = orig;
            parameters.desiredCurvature = 0.001;
            parameters.stiffLengthRatioCircum = 0.1;
            parameters.stiffLengthRatioRadial = 1;
            parameterList.push_back(parameters);

    massCalcer.calculateAll(parameterList);
    return 0;
}
