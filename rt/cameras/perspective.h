#ifndef CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER
#define CG1RAYTRACER_CAMERAS_PERSPECTIVE_HEADER

#include <rt/cameras/camera.h>
#include <core/vector.h>
#include <core/point.h>
#include <rt/mediums/medium.h>

namespace rt {

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle,
		Medium *medium
    );

	PerspectiveCamera(
        const Point& center,
        const Vector& forward,
        const Vector& up,
        float verticalOpeningAngle,
        float horizonalOpeningAngle
	);

	virtual Ray getPrimaryRay(float x, float y) const;

private:
	Point center;
	Vector forward;
	Vector yDir;
	Vector xDir;
	Vector camDir;
	Vector up;
	float verticalOpeningAngle;
	float horizontalOpeningAngle;
	Vector spanVectLengthHor;
	Vector spanVectLengthVert;

};

}


#endif