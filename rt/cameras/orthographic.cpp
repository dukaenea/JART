#include <rt/cameras/orthographic.h>
#include <rt/ray.h>

namespace rt {

OrthographicCamera::OrthographicCamera(const Point& center, const Vector& forward, const Vector& up, float scaleX, float scaleY)
{
	this->center = center;
	this->up = up;
	this->forward = forward;
	this->camDir = (forward).normalize();
	this-> xDir = cross(camDir,up).normalize();
	this-> yDir = cross(camDir, xDir).normalize();
	this->scaleX = scaleX;
	this->scaleY = scaleY;
}

Ray OrthographicCamera::getPrimaryRay(float x, float y) const {
	return Ray(center + x * scaleX * 0.5f * xDir - y * scaleY * 0.5f * yDir, forward);
}

}
