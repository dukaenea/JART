#include <rt/cameras/dofperspective.h>
#include <rt/ray.h>
#include <math.h>
#include <rt/solids/disc.h>

namespace rt {

DOFPerspectiveCamera::DOFPerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, float focalDistance, float apertureRadius)
{
    this->center = center;
	this->forward = forward;// (forward+Point(0,0,0))-center;
	this->up = up;
	this->verticalOpeningAngle = verticalOpeningAngle;
	this->horizontalOpeningAngle = horizontalOpeningAngle;
    this->focalDistance = focalDistance;
    this->apatureRadius = apertureRadius;
	this->camDir = (-this->forward).normalize();
	this->xDir = cross(camDir,up).normalize();
	this->yDir = cross(camDir, xDir);
	float focalLength = this->forward.length();
	this->spanVectLengthHor = focalLength * tanf(horizontalOpeningAngle * 0.5f) * xDir;
	this->spanVectLengthVert = focalLength * tanf(verticalOpeningAngle * 0.5f) *  yDir;
}

Ray DOFPerspectiveCamera::getPrimaryRay(float x, float y) const {
    Point focusPoint = this->center + this->focalDistance * (this->forward - x * spanVectLengthHor - y * spanVectLengthVert);
    Point sample = Disc(this->center, this->forward, 2*this->apatureRadius, nullptr, nullptr).sample().point;
    return Ray(sample, (focusPoint - sample).normalize());
}

}