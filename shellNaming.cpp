#include "shellNaming.h"

ShellName::ShellName()
{
}

ShellName::~ShellName()
{
}

std::string ShellName::makeName(ShellParams &parameters)
{   
    std::string name = "IR" + std::to_string(parameters.initRadius) + " Ex" + std::to_string(parameters.expansions) + " ExL" + std::to_string(parameters.extensionLength) + " S" + std::to_string(parameters.stiffnessCoef) + " LP" + std::to_string(parameters.lengthStiffnessCoef)+ " DC" + std::to_string(parameters.desiredCurvature);
    return name;
}
