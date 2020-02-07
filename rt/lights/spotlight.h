#ifndef CG1RAYTRACER_LIGHTS_SPOTLIGHT_HEADER
#define CG1RAYTRACER_LIGHTS_SPOTLIGHT_HEADER

#include <core/scalar.h>
#include <core/vector.h>
#include <rt/lights/pointlight.h>

namespace rt {

class SpotLight : public PointLight {
public:
	SpotLight() {}
	SpotLight(const Point& position, const Vector& direction, float angle, float exp, const RGBColor& intensity);
	SpotLight(const Point& position, const Vector& direction, float angle, float exp, const RGBColor& intensity, Medium* medium);
    virtual RGBColor getIntensity(const LightHit& irr) const;
	virtual LightHit getLightHit(const Point& p)const;


private:
	Point pos;
	Vector dir;
	float angle;
	float exp;
	RGBColor intensity;
};

}

#endif
