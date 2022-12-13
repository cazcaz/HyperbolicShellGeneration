#include "shellNaming.h"

ShellName::ShellName()
{
}

ShellName::~ShellName()
{
}

std::string ShellName::makeName(ShellParams &parameters)
{   
    std::string name = "Ex" + std::to_string(parameters.expansions) + " ExL" + std::to_string(parameters.extensionLength) + " SLRC" + std::to_string(parameters.stiffLengthRatioCircum) + " SLRR" + std::to_string(parameters.stiffLengthRatioRadial) + " DC" + std::to_string(parameters.desiredCurvature);
    return name;
}
