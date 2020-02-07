#ifndef CG1RAYTRACER_BBOX_HEADER
#define CG1RAYTRACER_BBOX_HEADER

#include <utility>
#include <core/point.h>
#include <core/vector.h>
#include <rt/ray.h>

namespace rt {

class Ray;

class BBox {
public:
    Point min, max;

    BBox() {}
    BBox(const Point& min, const Point& max)
    {
        // TODO maybe we should check which of the ,points is smaller so we know which one is the min
       this->min = min;
       this->max = max;
    }

    static BBox empty();
    static BBox full();

    void extend(const Point& point);
    void extend(const BBox& bbox);

    Vector diagonal() const {
        return this->max-this->min;
    }

    std::pair<float, float> intersect(const Ray& ray) const;
    float getSurfaceArea();

    bool isUnbound();
    bool isEmpty() const;
};

}

#endif