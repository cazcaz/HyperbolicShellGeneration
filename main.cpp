#include <iostream>
#include "shellGen.h"
#include <Eigen/Core>
#include <vector>
#include "energyFunction.h"
#include "circleGen.h"

using Eigen::Vector3d;
using Eigen::VectorXd;

int main(int, char**) {
    ShellGen shellGenerator;
    shellGenerator.setInitCurve(40, 0,0,0, 100);
    shellGenerator.expandCurveNTimes(100, 1, 10, 100);
    shellGenerator.printSurface("FirstSurfaceWithWorkingEnergy");

    // using this to test the energy function

    // CircleGen circleMaker;
    // double length = 1.5;
    // std::vector<Vector3d> prevcurve;
    // std::vector<Vector3d> normals;
    // std::vector<Vector3d> binormals;
    // circleMaker.makeCircle(1, Vector3d(0,0,0), 100, prevcurve);
    // circleMaker.makeCircle(1, Vector3d(0,0,0), 100, normals);
    // for (Vector3d point : prevcurve) {
    //     binormals.push_back(Vector3d(0,0,1));
    // }
    // VectorXd inputs = VectorXd::Zero(100);
    // VectorXd inputsChanged = VectorXd::Zero(100);
    // VectorXd derivs = VectorXd::Zero(100);
    // VectorXd derivsAfter = VectorXd::Zero(100);
    // inputs[9] = 0.1;
    // inputs[10] = 0.5;
    // inputs[11] = 0.1;
    // inputsChanged[9] = 0.1;
    // inputsChanged[10] = 0.5;
    // inputsChanged[11] = 0.1;
    // double h = 0.000000005;
    // inputsChanged[10] += h;
    // EnergyFunction energyFunc(prevcurve, normals, binormals, length, 10, 100, 1 + length, 1);
    // std::cout << energyFunc(inputs, derivs) << std::endl;
    // std::cout << (energyFunc(inputsChanged, derivsAfter) - energyFunc(inputs, derivs))/h << std::endl;
    // std::cout << derivs[10] << std::endl;
    return 0;
}
