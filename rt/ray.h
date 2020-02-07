#ifndef CG1RAYTRACER_RAY_HEADER
#define CG1RAYTRACER_RAY_HEADER

#include <core/vector.h>
#include <core/point.h>

namespace rt {
class Medium;
class Ray {
public:

    Point o;
    Vector d;
    Vector rayOrigin;
    const Medium* medium;


    Ray() {}
    Ray(const Point& o, const Vector& d);
    Ray(const Point& o, const Vector& d, const Medium* medium);
    Point getPoint(float distance) const;
};

}

#endif