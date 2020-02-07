#ifndef CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_SPHERICAL_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/vector.h>
#include <core/matrix.h>

namespace rt {

class Vector;

class SphericalCoordMapper : public CoordMapper {
public:
    SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef);
    virtual Point getCoords(const Intersection& hit) const;

private:
    Point center;
    Vector zenith;
    Vector azimuthRef;
    Matrix base;

    float scaleX;
    float scaleY;


};

}

#endif