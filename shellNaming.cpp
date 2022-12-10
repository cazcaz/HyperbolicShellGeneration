#include "shellNaming.h"

ShellName::ShellName()
{
}

ShellName::~ShellName()
{
}

std::string ShellName::makeName(ShellParams &parameters)
{   
    std::string name = "Ex" + std::to_string(parameters.expansions) + " ExL" + std::to_string(parameters.extensionLength) + " SLR" + std::to_string(parameters.stiffLengthRatio) + " DC" + std::to_string(parameters.desiredCurvature);
    return name;
}
