#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>
#include <core/random.h>
#include <core/scalar.h>

namespace rt {

class Disc : public Solid {
public:

    Point origin;
    Vector normal;
    float radius;
    CoordMapper* mapper;
    Material* material;

    Disc() {}
    Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    virtual void setMaterial(Material* m) {this->material = m; }
};

}

#endif