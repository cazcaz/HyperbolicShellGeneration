#pragma once
#include "point3D.h"

class VectorMaths {
    public:
        VectorMaths();
        ~VectorMaths();

        double dot(Point3D p1 , Point3D p2);
        Point3D cross(Point3D p1 , Point3D p2);
        void normalise(Point3D* p1);
    private:

};