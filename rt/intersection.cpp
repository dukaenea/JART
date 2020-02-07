#include <rt/intersection.h>

namespace rt {

Intersection::Intersection(float distance, const Ray& ray, const Solid* solid, const Vector& normal, const Point& local)
{
    this->distance = distance;
    this->ray = ray;
    this->solid = solid;
    this->nrm = normal;
    this->loc = local;
}

Intersection::operator bool() {
    return this->solid != nullptr;
}

Intersection Intersection::failure() {
    return Intersection(FLT_MAX, Ray(), nullptr, Vector(), Point());
}

Point Intersection::hitPoint() const {
    return this->ray.getPoint(this->distance);
}

Vector Intersection::normal() const {
    return this->nrm;
}

Point Intersection::local() const {
    return this->loc;
}

}
