#ifndef CG1RAYTRACER_COORDMAPPERS_ENVIRONMENTAL_HEADER
#define CG1RAYTRACER_COORDMAPPERS_ENVIRONMENTAL_HEADER

#include <rt/coordmappers/coordmapper.h>
namespace rt {

class EnvironmentalCoordMapper : public CoordMapper {
public:
    EnvironmentalCoordMapper(){};
    virtual Point getCoords(const Intersection& hit) const;
};

}

#endif