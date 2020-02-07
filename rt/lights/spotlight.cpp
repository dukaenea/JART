#include <rt/lights/spotlight.h>
#include <math.h>

namespace rt {

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity, Medium *medium = nullptr)
{
    this->pos = position;
    this->dir = direction;
    this->angle = angle;
    this->exp = power;
    this->intensity = intensity;
    this->medium = medium;
}

SpotLight::SpotLight(const Point& position, const Vector& direction, float angle, float power, const RGBColor& intensity)
{
    this->pos = position;
    this->dir = direction;
    this->angle = angle;
    this->exp = power;
    this->intensity = intensity;
}


LightHit SpotLight::getLightHit(const Point& p) const {
    LightHit lh = LightHit();
    
    Vector connLine = this->pos - p;
    lh.distance = connLine.length(); 
    lh.direction = connLine.normalize();

    return lh;
}

RGBColor SpotLight::getIntensity(const LightHit& irr) const {
    
    Vector spDir = this->dir.normalize();
    float angle = acosf(dot(spDir, -irr.direction));

    if(angle > this->angle){
        return RGBColor::rep(0);
    }

    float cos = dot(spDir, -irr.direction);

    return this->intensity * powf(cos, exp) / powf(irr.distance, 2);



}

}
