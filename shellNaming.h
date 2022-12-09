#pragma once

#include <string>
#include "shellParams.h"

class ShellName {
    public:
        ShellName();
        ~ShellName();
        std::string makeName(ShellParams& parameters);
    private:
};