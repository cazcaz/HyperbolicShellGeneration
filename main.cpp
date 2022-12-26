#include "batchGen.h"

int main(int, char**) {
    
    //To use, populate parameterList with parameters of surfaces to be calculated, then call batchGen to calcuate them

    //std::vector<ShellParams> parameterList;
    //BatchGen massCalcer;

    //parameterList.push_back({});

    //Push parameters to the parameterList

    //massCalcer.calculateAll(parameterList);

    // For benchmarking


    for (int i = 0; i<1000; i++){
        ShellParams parameters;
        parameters.centreX = 0;
        parameters.centreY = 0;
        parameters.centreZ = 0;
        parameters.desiredCurvature = 0.0001;
        parameters.expansions = 100;
        parameters.resolution = 100;
        parameters.extensionLength = 0.1;
        parameters.stiffLengthRatioCircum = 0.1;
        parameters.stiffLengthRatioRadial = 0.1;
        ShellGen shellGenerator(parameters);
        shellGenerator.setInitCurve();
        shellGenerator.expandCurveNTimes();
    }
    
    return 0;
}
