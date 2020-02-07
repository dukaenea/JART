#include <rt/solids/sphere.h>

namespace rt {

Sphere::Sphere(const Point& center, float radius, CoordMapper* texMapper, Material* material)
{
    this->center = center;
	this->radius = radius;
	this->texMapper = texMapper;
	this->material = material;
}

BBox Sphere::getBounds() const {
    return BBox(
		Point(
			this->center.x - this->radius,
			this->center.y - this->radius,
			this->center.z - this->radius
		),
		Point(
			this->center.x + this->radius,
			this->center.y + this->radius,
			this->center.z + this->radius
		)
	);
}

Intersection Sphere::intersect(const Ray& ray, float previousBestDistance) const {

    Vector originVector = this->center - Point::rep(0.0f);

    float a = 1;
	//float b = 2 * dot(ray.d,(ray.rayOrigin-originVector));

	float b = 2 * dot((ray.o-this->center),ray.d);

	float c = (ray.o-this->center).lensqr() - (this->radius*this->radius);

	float discriminant = b * b - 4 * c;

	if (discriminant >= 0) {
		// the ray intersects the sphere

		float p1 = ((-1 * b - sqrtf(discriminant)) / 2) - 0.00001;

		if (p1 > 0) {
			//this is the first intersection

			if(p1 > previousBestDistance ||  p1 < 0){
				return Intersection::failure();
			}

			Point interSectionPoint = ray.getPoint(p1);

			return Intersection(p1, ray, this, ((interSectionPoint - this->center)).normalize(), interSectionPoint);

		}
		else {

			float p2 = (( sqrtf(discriminant) - b) / 2) - 0.00001;

			if(p2 > previousBestDistance ||  p2 < 0){
				return Intersection::failure();
			}

			
			Point interSectionPoint = ray.getPoint(p2);

			return Intersection(p2, ray, this, ((interSectionPoint - this->center)).normalize(), interSectionPoint);

		}
	}

	return Intersection::failure();

}

Solid::Sample Sphere::sample() const {
	NOT_IMPLEMENTED;
}

float Sphere::getArea() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}
