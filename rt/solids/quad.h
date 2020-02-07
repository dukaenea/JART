#ifndef CG1RAYTRACER_SOLIDS_QUAD_HEADER
#define CG1RAYTRACER_SOLIDS_QUAD_HEADER

#include <rt/solids/solid.h>
#include <core/random.h>

namespace rt {

class Quad : public Solid {
public:


    Point origin;
	Vector span1;
	Vector span2;
	CoordMapper* textMapper;
    
    Quad() {}
    Quad(const Point& v1, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material);

    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual Sample sample() const;
    virtual float getArea() const;
    Point getLocalIntersectionPoint(const Ray& ray) const;
    virtual void setMaterial(Material* m) {this->material = m; }
};

}

#endif