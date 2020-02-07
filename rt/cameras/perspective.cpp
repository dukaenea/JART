#include <rt/cameras/perspective.h>
#include <math.h>

namespace rt {

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle, Medium* medium = nullptr)
{
	this->center = center;
	this->forward = forward;// (forward+Point(0,0,0))-center;
	this->up = up;
	this->verticalOpeningAngle = verticalOpeningAngle;
	this->horizontalOpeningAngle = horizontalOpeningAngle;
	this->camDir = (-this->forward).normalize();
	this->xDir = cross(camDir,up).normalize();
	this->yDir = cross(camDir, xDir);
	float focalLength = this->forward.length();
	this->spanVectLengthHor = focalLength * tanf(horizontalOpeningAngle * 0.5f) * xDir;
	this->spanVectLengthVert = focalLength * tanf(verticalOpeningAngle * 0.5f) *  yDir;
	this->medium = medium;
}

PerspectiveCamera::PerspectiveCamera(const Point& center, const Vector& forward, const Vector& up, float verticalOpeningAngle, float horizontalOpeningAngle)
{
	this->center = center;
	this->forward = forward;// (forward+Point(0,0,0))-center;
	this->up = up;
	this->verticalOpeningAngle = verticalOpeningAngle;
	this->horizontalOpeningAngle = horizontalOpeningAngle;
	this->camDir = (-this->forward).normalize();
	this->xDir = cross(camDir,up).normalize();
	this->yDir = cross(camDir, xDir);
	float focalLength = this->forward.length();
	this->spanVectLengthHor = focalLength * tanf(horizontalOpeningAngle * 0.5f) * xDir;
	this->spanVectLengthVert = focalLength * tanf(verticalOpeningAngle * 0.5f) *  yDir;
}

Ray PerspectiveCamera::getPrimaryRay(float x, float y) const {
	return Ray(this->center, (this->forward - x * spanVectLengthHor - y * spanVectLengthVert ).normalize(), this->medium != nullptr ? medium : nullptr);
}

}
