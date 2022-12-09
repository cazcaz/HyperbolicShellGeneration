#pragma once


class ShellParams {
  public:
    ShellParams();
    ~ShellParams();
    double centreX = 0;
    double centreY = 0;
    double centreZ = 0;
    double initRadius = 40;
    int resolution = 100;
    int expansions = 1000;
    double extensionLength = 1;
    double stiffnessCoef = 100;
    double lengthStiffnessCoef = 1000;
    double desiredCurvature = 2;
    private:

};