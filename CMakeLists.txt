cmake_minimum_required(VERSION 3.0.0)
project(ShellGeneration VERSION 0.1.0)

include_directories(LBFGSpp)

add_library(LibsModule
     point3D.cpp
     vectorMaths.cpp
     circleGen.cpp
     shellGen.cpp
     LBFGS.h
     LBFGSB.h
     LBFGSpp/Param.h
     LBFGSpp/BFGSMat.h
     LBFGSpp/Cauchy.h
     LBFGSpp/SubspaceMin.h
     LBFGSpp/LineSearchMoreThuente.h
     LBFGSpp/LineSearchBacktracking.h
     LBFGSpp/LineSearchBracketing.h
     LBFGSpp/LineSearchNocedalWright.h
     LBFGSpp/BKLDLT.h
)

add_executable(ShellGeneration main.cpp)


target_link_libraries(ShellGeneration LibsModule)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
