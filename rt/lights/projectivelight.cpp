#include <rt/lights/projectivelight.h>
#include <core/scalar.h>
#include <core/color.h>
#include <math.h>

//rt::RGBColor a1computeColor(uint x, uint y, uint width, uint height);
rt::RGBColor a1computeColor(rt::uint x, rt::uint y, rt::uint width, rt::uint height);
namespace rt {

ProjectiveLight::ProjectiveLight(const Point& position, const RGBColor& intensity)
{
    this->pos = position;
    this->intensity = intensity;
}


LightHit ProjectiveLight::getLightHit(const Point& p) const {
    LightHit lh = LightHit();
    
    Vector connLine = this->pos - p;
    lh.distance = connLine.length(); 
    lh.direction = connLine.normalize();

    return lh;
}

RGBColor ProjectiveLight::getIntensity(const LightHit& irr) const {

    //trying to map the world into a cilinder whose center is the same as the lght position
    int resolution = 2000;  
    int h = 3000;
    float radius = resolution / (2 * __PIE__);
    float angleX = std::acos(dot(Vector(irr.direction.x, irr.direction.y, this->pos.z).normalize(), Vector(0,1,0)));
    float angelY = std::acos(dot(Vector(this->pos.x, irr.direction.y, irr.direction.z).normalize(), Vector(0,1,0)));
    float up = radius * std::tan(angelY) + resolution/2;
    uint x = (uint)(resolution * angleX / (2 * __PIE__));
    uint y = (uint)(up);

    RGBColor c = a1computeColor(x, y, resolution, resolution);

    return (this->intensity / (irr.distance * irr.distance)) * c;// * a1computeColor(x, y, resolution, resolution));
}

}
