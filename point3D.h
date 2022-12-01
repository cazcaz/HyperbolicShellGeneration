#pragma once
#include <iostream>

class Point3D {
    public:
        Point3D(double x=0, double y=0, double z=0);
        ~Point3D();
        Point3D operator+(const Point3D &rhs);
        Point3D operator-(const Point3D &rhs);
        Point3D operator*(const double &rhs);
        Point3D operator/(const double &rhs);
        friend std::ostream& operator<<(std::ostream& os, Point3D const &point){
            os << "{" << point.m_x << ", " << point.m_y << ", " << point.m_z << "}";
            return os;
        };
        double getX();
        double getY();
        double getZ();
        double norm();
    private:
        double m_x;
        double m_y;
        double m_z;

};