#pragma once

struct ShellParams {
  double centreX = 0;
    double centreY = 0;
    double centreZ = 0;
    int resolution = 100;
    int expansions = 1000;
    double extensionLength = 1;
    double stiffLengthRatio = 1;
    double desiredCurvature = 2;
};
