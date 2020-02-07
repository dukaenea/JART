#include <rt/integrators/casting.h>
#include <rt/intersection.h>
#include <rt/world.h>


namespace rt {

RGBColor RayCastingIntegrator::getRadiance(const Ray& ray) const {
    Intersection intersection = this->world->scene->intersect(ray);
    if(intersection){
        float grayColor = dot(-intersection.ray.d, intersection.normal());
        return RGBColor(grayColor, grayColor, grayColor);
    }
    else{
        return RGBColor::rep(0.0f);
    }
}

}
