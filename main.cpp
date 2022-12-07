#include <iostream>
#include "shellGen.h"
#include <Eigen/Core>
#include <vector>
#include "energyFunction.h"
#include "circleGen.h"

using Eigen::Vector3d;
using Eigen::VectorXd;

int main(int, char**) {
    double initRadius = 40;
    int resolution = 100;
    int expansions = 1000;
    double extensionLength = 1;
    double stiffnessCoef = 100;
    double lengthStiffnessCoef = 1000;
    double desiredCurvature = 2;

    ShellGen shellGenerator;
    shellGenerator.setInitCurve(initRadius, 0,0,0, resolution);
    shellGenerator.expandCurveNTimes(expansions, extensionLength, stiffnessCoef, lengthStiffnessCoef, desiredCurvature);
    std::string autoName = "IR" + std::to_string(initRadius) + " Ex" + std::to_string(expansions) + " ExL" + std::to_string(extensionLength) + " S" + std::to_string(stiffnessCoef) + " LP" + std::to_string(lengthStiffnessCoef)+ " DC" + std::to_string(desiredCurvature);
    shellGenerator.printSurface(autoName);
    return 0;
}
