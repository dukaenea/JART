#include <rt/lights/directional.h>
#include <core/scalar.h>
#include <core/vector.h>

namespace rt {

DirectionalLight::DirectionalLight(const Vector& direction, const RGBColor& color)
{
    this->dir = direction;
    this->color = color;
}

LightHit DirectionalLight::getLightHit(const Point& p) const {
    return {-this->dir,__FLT_MAX__, {}};     
}

RGBColor DirectionalLight::getIntensity(const LightHit& irr) const {
    return this->color;
}

}
