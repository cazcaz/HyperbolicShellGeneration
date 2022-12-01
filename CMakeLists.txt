cmake_minimum_required(VERSION 3.0.0)
project(ShellGeneration VERSION 0.1.0)

add_library(LibsModule
     point3D.cpp
     vectorMaths.cpp
     circleGen.cpp
     shellGen.cpp
)

add_executable(ShellGeneration main.cpp)

include(ExternalProject)

ExternalProject_Add (
       B
      GIT_REPOSITORY "https://github.com/yixuan/LBFGSpp/"
      GIT_TAG "563106b"
      UPDATE_COMMAND ""
      INSTALL_COMMAND "" )


target_link_libraries(ShellGeneration LibsModule)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
