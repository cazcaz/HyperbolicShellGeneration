#pragma once
#include <iostream>

class Point3D {
    public:
        Point3D(float x, float y, float z);
        ~Point3D();
        Point3D operator+(const Point3D &rhs);
        Point3D operator-(const Point3D &rhs);
        Point3D operator*(const float &rhs);
        Point3D operator/(const float &rhs);
        friend std::ostream& operator<<(std::ostream& os, Point3D const &point){
            os << "{" << point.m_x << ", " << point.m_y << ", " << point.m_z << "}";
            return os;
        };
        float getX();
        float getY();
        float getZ();
        float norm();
    private:
        float m_x;
        float m_y;
        float m_z;

};