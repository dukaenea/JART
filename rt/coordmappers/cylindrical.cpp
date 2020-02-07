#include <rt/coordmappers/cylindrical.h>
#include <rt/intersection.h>
#include <core/scalar.h>

namespace rt {

CylindricalCoordMapper::CylindricalCoordMapper(const Point& origin, const Vector& longitudinalAxis, const Vector& polarAxis)
{
    this->center = origin;
    this->primatryAxis = longitudinalAxis.normalize();
    this->polarAxis = polarAxis.normalize();


    Vector y = this->primatryAxis;
    Vector z = cross(this->polarAxis, y).normalize();     
    Vector x = cross(y, z).normalize();

    this->base = Matrix::system(x,y,z).invert();

    this->scaleX = polarAxis.length();
    this->scaleY = longitudinalAxis.length();

}

Point CylindricalCoordMapper::getCoords(const Intersection& hit) const {
    Vector hitOrigin = this->center - hit.local();
    Vector tranvec = Vector(base * Float4(hitOrigin));
    float dist = tranvec.length();
    //finding the angle between the transformed vector and the x axis of the new base
    float xAngle = std::atan(tranvec.z / tranvec.x);
    return Point(
        xAngle*(2 / pi) * scaleX,
        tranvec.y / scaleY,
        0.f
    );
}

}