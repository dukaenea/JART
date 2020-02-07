#ifndef CG1RAYTRACER_SOLIDS_DISC_HEADER
#define CG1RAYTRACER_SOLIDS_DISC_HEADER

#include <rt/solids/solid.h>

namespace rt {

class Environment : public Solid {
public:

    Environment() {};
    Environment(CoordMapper* texMapper);
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    virtual void setMaterial(Material* m) {this->material = m; }
};

}

#endif