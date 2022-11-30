#pragma once

#include <vector>
#include "point3D.h"

class Curve {
    public:
        Curve(int resolution);
        ~Curve();

        friend std::ostream& operator<<(std::ostream& os, Curve const &curve){
            os << "{";
            for (int i=0; i< curve.m_resolution - 1; i++){
                os << curve.m_points[i] << ", ";
            };
            os << curve.m_points[curve.m_resolution - 1] << "}";
            return os;
        };

        int getResolution();
        void setValue(int index, Point3D point);
        Point3D getValue(int index);
    private:
        int resolveIndex(int index);
        std::vector<Point3D> m_points;
        int m_resolution;
};