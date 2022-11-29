#pragma once
#include "vectorMaths.h"

VectorMaths::VectorMaths() {};

VectorMaths::~VectorMaths() {};

float VectorMaths::dot(Point3D p1 , Point3D p2){
    return p1.getX() * p2.getX() + p1.getY() * p2.getY() + p1.getZ() * p2.getZ();
};
        
Point3D VectorMaths::cross(Point3D p1 , Point3D p2){
    float newX = p1.getY()*p2.getZ() - p2.getY()*p1.getZ();
    float newY = p1.getZ()*p2.getX() - p2.getZ()*p1.getX();
    float newZ = p1.getX()*p2.getY() - p2.getX()*p1.getY();
    return Point3D(newX, newY, newZ);
};
        
void VectorMaths::normalise(Point3D* p1){
    float norm = p1->norm();
    *p1 = *p1 / norm;
};


