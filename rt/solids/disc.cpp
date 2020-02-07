#include <rt/solids/disc.h>
#include <math.h>

namespace rt {

Disc::Disc(const Point& center, const Vector& normal, float radius, CoordMapper* texMapper, Material* material)
{
    this->origin = center;
    this->normal = normal;
    this->radius = radius;
    this->mapper = texMapper;
    this->material = material;

}

BBox Disc::getBounds() const {
    return BBox(
		Point(
			this->origin.x - this->radius,
			this->origin.y - this->radius,
			this->origin.z - this->radius
		),
		Point(
			this->origin.x + this->radius,
			this->origin.y + this->radius,
			this->origin.z + this->radius
		)
	);
}

Intersection Disc::intersect(const Ray& ray, float previousBestDistance) const {
    Vector op = this->origin - ray.o;

	float rayNormal = dot(ray.d, normal);

	if (rayNormal == 0) {
		return Intersection::failure();
	}

	float dist = dot(op, normal) / dot(ray.d, normal);

	Point ip = ray.getPoint(dist);

	if (dist < previousBestDistance && dist>0 && (ip-this->origin).length()<=this->radius) {

		return Intersection(dist, ray, this, (this->normal), ip);
	}
	else {
		return Intersection::failure();
	}

}

Solid::Sample Disc::sample() const {
   Solid::Sample sample;

	//we can use the polar coordinates to get a random distance between 0 and radius and an angle between 0 and 2pie
	float angle = random(2*pi);
	float distance = random(this->radius);
	//we need to find a vector on the disk that is perp. to the normal
	 Vector bigClockHandle;
	 if(std::fabs(this->normal.x) >= std::fabs(this->normal.y) && std::fabs(this->normal.x) >= std::fabs(this->normal.z)){
		 bigClockHandle = Vector(0.f, -this->normal.z, this->normal.y);
	 }
	 else if(std::fabs(this->normal.x) >= std::fabs(this->normal.y)){
		 bigClockHandle = Vector(-this->normal.z, 0.f, this->normal.x);
	 }
	 else{
		 bigClockHandle = Vector(-this->normal.y, this->normal.x, 0.f);
	 }


	Vector smallClockHandle = cross(this->normal, bigClockHandle);
	sample.normal = this->normal;
	sample.point = this->origin + distance * (bigClockHandle * std::sin(angle) + smallClockHandle * std::cos(angle)).normalize();
	return sample;

}

float Disc::getArea() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}
