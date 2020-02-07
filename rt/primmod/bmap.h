#ifndef CG1RAYTRACER_PRIMMOD_BMAP_HEADER
#define CG1RAYTRACER_PRIMMOD_BMAP_HEADER

#include <rt/primitive.h>
#include<vector>
#include <rt/solids/striangle.h>

namespace rt {

class Triangle;
class Texture;
class SmoothTriangle;

enum MappingType{
    BUMP,
    NORMAL
};


class BumpMapper : public Primitive {
public:
    BumpMapper(SmoothTriangle* base, Texture* bumpmap, const Point& bv1, const Point& bv2, const Point& bv3, MappingType mappingtype, float vscale = 0.f, bool flipgreen = false);
    virtual BBox getBounds() const;
    virtual Intersection intersect(const Ray& ray, float previousBestDistance = FLT_MAX) const;
    virtual void setMaterial(Material* m);
    virtual void setCoordMapper(CoordMapper* cm);
    
private:
    SmoothTriangle* base;
    Texture* bumpmap;
    Point verteces[3];
    float vScale;
    bool flipGreenChannel; //sometimes the normal map from the models has the green channel already flipped so we might need to flip back
    MappingType mappingtype;
};

}

#endif