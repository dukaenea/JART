#ifndef CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_CYLINDRICAL_HEADER

#include <rt/coordmappers/coordmapper.h>
#include <core/vector.h>
#include <core/matrix.h>

namespace rt {

class Vector;

class CylindricalCoordMapper : public CoordMapper {
public:
    CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis);
    virtual Point getCoords(const Intersection& hit) const;

private:
    Point center;
    Vector primatryAxis;
    Vector polarAxis;
    Matrix base;

    float scaleX;
    float scaleY;

};

}

#endif