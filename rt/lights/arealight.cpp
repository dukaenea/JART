#include <rt/lights/arealight.h>

#include <core/color.h>

namespace rt {

LightHit AreaLight::getLightHit(const Point& p) const {
    LightHit lh;
    Solid::Sample samplePoint = this->light->sample();
    Vector connLine = samplePoint.point - p;

    //accounting for calculation errors, decreasing the distance as little as possible
    float k = 1e-3;

    lh.direction = connLine.normalize();
    lh.distance = connLine.length() - k;
    lh.normal = samplePoint.normal;

    return lh;
}

RGBColor AreaLight::getIntensity(const LightHit& irr) const {
    //return RGBColor::rep(0);
    RGBColor c = this->light->material->getEmission(Point::rep(0), Vector::rep(0), Vector::rep(0)) * std::abs((dot(irr.normal, irr.direction)/(irr.distance*irr.distance))) * this->light->getArea();
    return c;
}

AreaLight::AreaLight(Solid* source)
{
    this->light = source;
}

}