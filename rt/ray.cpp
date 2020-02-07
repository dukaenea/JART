#include <core/assert.h>
#include <core/macros.h>
#include <rt/ray.h>
#include <cmath>
#include <rt/mediums/medium.h>

namespace rt {

Ray::Ray(const Point& o, const Vector& d)
{
    assert(d != Vector::rep(0.0f));
	this->o = o;
	this->d = d;
	this-> rayOrigin = o - Point(0, 0, 0);
}


Ray::Ray(const Point& o, const Vector& d, const Medium* medium)
{
    assert(d != Vector::rep(0.0f));
	this->o = o;
	this->d = d;
	this->rayOrigin = o - Point(0, 0, 0);
	this->medium = medium;
}



Point Ray::getPoint(float distance) const {
	if (fabs(distance) <= __FLT_EPSILON__) {
		return o;
	}

	return d * distance + o;
}

}
