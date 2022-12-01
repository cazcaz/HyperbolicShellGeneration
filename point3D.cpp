#pragma once
#include "point3D.h"
#include <cmath>

Point3D::Point3D(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {};

Point3D::~Point3D() {};

Point3D Point3D::operator+(const Point3D &rhs){
    Point3D temp(m_x + rhs.m_x, m_y + rhs.m_y, m_z + rhs.m_z);
    return temp;
};

Point3D Point3D::operator-(const Point3D &rhs){
    Point3D temp(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
    return temp;
};

Point3D Point3D::operator*(const double &rhs){
    Point3D temp(m_x * rhs, m_y * rhs ,m_z * rhs);
    return temp;
};

Point3D Point3D::operator/(const double &rhs){
    Point3D temp(m_x / rhs, m_y / rhs ,m_z / rhs);
    return temp;
};

double Point3D::getX(){
    return m_x;
};

double Point3D::getY(){
    return m_y;
};

double Point3D::getZ(){
    return m_z;
};

void Point3D::setX(double x) {
    m_x = x;
}

void Point3D::setY(double y) {
    m_y = y;
}

void Point3D::setZ(double z) {
    m_z = z;
}

double Point3D::norm(){
    return sqrt(pow(m_x,2)+pow(m_y,2)+pow(m_z,2));
};




