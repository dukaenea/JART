#include <rt/lights/pointlight.h>

#include <core/vector.h>
#define LOG(x) std::cout<<x<<std::endl;
namespace rt {

PointLight::PointLight(const Point& position, const RGBColor& intensity)
{
    this->position = position;
    this->intensity = intensity;
}

LightHit PointLight::getLightHit(const Point& p) const {
    LightHit lh = LightHit();
    
    Vector connLine = this->position - p;
    lh.distance = connLine.length(); 
    lh.direction = connLine.normalize();

    return lh;
}

RGBColor PointLight::getIntensity(const LightHit& irr) const {
    return this->intensity / (irr.distance * irr.distance);
}

}
