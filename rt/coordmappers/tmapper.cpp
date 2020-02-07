#include <rt/coordmappers/tmapper.h>
#include <rt/intersection.h>
#include <core/float4.h>

namespace rt {

TriangleMapper::TriangleMapper(Point ntv[3])
{
    this->tc1 = ntv[0];
    this->tc2 = ntv[1];
    this->tc3 = ntv[2];
}

TriangleMapper::TriangleMapper(const Point& tv0, const Point& tv1, const Point& tv2)
{
    this->tc1 = tv0;
    this->tc2 = tv1;
    this->tc3 = tv2;
}

Point TriangleMapper::getCoords(const Intersection& hit) const {
    Point p = tc1 * hit.local().x + tc2 * hit.local().y + tc3 * hit.local().z; 
    return p;
}

}