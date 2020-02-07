#include <rt/coordmappers/world.h>
#include <rt/intersection.h>

namespace rt {

Point WorldMapper::getCoords(const Intersection& hit) const {
    return Point(Float4(hit.local()) * this->scaleFactor);
}

WorldMapper::WorldMapper()
{
    /* TODO */
}

WorldMapper::WorldMapper(const Float4& scale)
{
    this->scaleFactor = scale;
}

}