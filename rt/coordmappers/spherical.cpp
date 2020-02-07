#include <rt/coordmappers/spherical.h>
#include <rt/intersection.h>
#include <core/scalar.h>

namespace rt {

SphericalCoordMapper::SphericalCoordMapper(const Point& origin, const Vector& zenith, const Vector& azimuthRef)
{
    this->center = origin;
    this->zenith = zenith.normalize();
    this->azimuthRef = azimuthRef.normalize();


    Vector y = this->zenith;
    Vector z = cross(this->azimuthRef, y).normalize();     
    Vector x = cross(y, z).normalize();

    this->base = Matrix::system(x,y,z).invert();

    this->scaleX = zenith.length();
    this->scaleY = azimuthRef.length();
}

Point SphericalCoordMapper::getCoords(const Intersection& hit) const {
    Vector hitOrigin = this->center - hit.local();
    Vector tranvec = Vector(base * Float4(hitOrigin));
    float dist = tranvec.length();
    //finding the angle between the transformed vector and the x axis of the new base
    float xAngle = std::atan(tranvec.z / tranvec.x);
    //finding the angle between the transformed vector and the y axis of the new base
    float yAngle = std::acos(tranvec.y / dist);
    return Point(
        xAngle*(2 / pi) * scaleX,
        yAngle * (1 / pi) / scaleY,
        0.f
    );
}

}