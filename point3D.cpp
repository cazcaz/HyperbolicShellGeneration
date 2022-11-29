#pragma once
#include "point3D.h"
#include <cmath>

Point3D::Point3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z) {};

Point3D::~Point3D() {};

Point3D Point3D::operator+(const Point3D &rhs){
    Point3D temp(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
    return temp;
};

Point3D Point3D::operator-(const Point3D &rhs){
    Point3D temp(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
    return temp;
};

Point3D Point3D::operator*(const float &rhs){
    Point3D temp(m_x * rhs, m_y * rhs ,m_z * rhs);
    return temp;
};

Point3D Point3D::operator/(const float &rhs){
    Point3D temp(m_x / rhs, m_y / rhs ,m_z / rhs);
    return temp;
};

float Point3D::getX(){
    return m_x;
};

float Point3D::getY(){
    return m_y;
};

float Point3D::getZ(){
    return m_z;
};

float Point3D::norm(){
    return sqrt(pow(m_x,2)+pow(m_y,2)+pow(m_z,2));
};




