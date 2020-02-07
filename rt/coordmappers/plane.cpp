#include <rt/coordmappers/plane.h>
#include <rt/intersection.h>
#include <core/matrix.h>

namespace rt {

PlaneCoordMapper::PlaneCoordMapper(const Vector& e1, const Vector& e2) {
    this->v1 = e1;
    this->v2 = e2;
}

Point PlaneCoordMapper::getCoords(const Intersection& hit) const {
    //we need to construct a new base that has out plane as one of it's primary planes
    Vector perp = cross(this->v2, this->v1);
    Matrix base = Matrix::system(this->v1, this->v2, perp).invert();
    return Point(base * Float4(hit.local()));
}

}