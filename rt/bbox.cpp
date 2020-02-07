#include <rt/bbox.h>

namespace rt {

BBox BBox::empty() {
    return BBox(Point::rep(__FLT_MAX__), Point::rep(-__FLT_MAX__));
}

BBox BBox::full() {
    return BBox(Point::rep(-__FLT_MAX__), Point::rep(__FLT_MAX__));
}


void BBox::extend(const Point& point) {
    this->min = Point(
        std::min(point.x, this->min.x),
        std::min(point.y, this->min.y),
        std::min(point.z, this->min.z)
    );

    this->max = Point(
        std::max(point.x, this->max.x),
        std::max(point.y, this->max.y),
        std::max(point.z, this->max.z)
    );
}

void BBox::extend(const BBox& bbox) {
    this->extend(bbox.min);
    this->extend(bbox.max);
}

std::pair<float, float> BBox::intersect(const Ray& ray) const {

    if(this->isEmpty()){
        return std::pair<float, float>(1.0f, 0.0f);
    }


    float rayDirX = 1.0f / ray.d.x;
    float rayDirY = 1.0f / ray.d.y;
    float rayDirZ = 1.0f / ray.d.z;

    float tmin = (min.x - ray.o.x) * rayDirX;
    float tmax = (max.x - ray.o.x) * rayDirX;

    if(tmin > tmax) std::swap(tmin, tmax);

    float ytmin = (min.y - ray.o.y) * rayDirY;
    float ytmax = (max.y - ray.o.y) * rayDirY;

    if(ytmin > ytmax) std::swap(ytmin, ytmax);

    if((tmin > ytmax) || (ytmin > tmax)) return std::make_pair(1.0f, 0.0f);

    if(ytmin > tmin) tmin = ytmin;
    if(ytmax < tmax) tmax = ytmax;

    float ztmin = (min.z - ray.o.z) * rayDirZ;
    float ztmax = (max.z - ray.o.z) * rayDirZ;

    if(ztmin > ztmax) std::swap(ztmin, ztmax);

    if((tmin > ztmax) || (ztmin > tmax)) return std::make_pair(1.0f, 0.0f);

    if(ztmin > tmin) tmin = ztmin;
    if(ztmax < tmax) tmax = ztmax;

    return std::make_pair(tmin, tmax);

}

bool BBox::isUnbound() {
    return min.x == -__FLT_MAX__ || min.y == -__FLT_MAX__ || min.z == -__FLT_MAX__ ||
            max.x == __FLT_MAX__ || max.y == __FLT_MAX__ || max.z == __FLT_MAX__;
}

bool BBox::isEmpty() const{
    return min.x == 0.0f && min.y == 0.0f && min.z == 0.0f && max.x == 0.0f && max.y == 0.0f && max.z == 0.0f;
}

float BBox::getSurfaceArea(){

    //calculating the lengths of the edges
    float xEdge = this->max.x - this->min.x;
    float yEdge = this->max.y - this->min.y;
    float zEdge = this->max.z - this->min.z;


    return (2.0f * xEdge * yEdge) + (2.0f * xEdge * zEdge) + (2.0f * yEdge * zEdge); 

}



}
