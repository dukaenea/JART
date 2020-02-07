#include <rt/solids/infiniteplane.h>

namespace rt {

InfinitePlane::InfinitePlane(const Point& origin, const Vector& normal, CoordMapper* texMapper, Material* material)
{
    this->origin = origin;
    this->normal = normal;
    this->texMapper = texMapper;
    this->material = material;
}

BBox InfinitePlane::getBounds() const {
    return BBox::full();
}

Intersection InfinitePlane::intersect(const Ray& ray, float previousBestDistance) const {
    //calculating the intersection point
    Vector op = this->origin - ray.o;
    Vector v =  Vector(ray.o.x, ray.o.y, ray.o.z);
    //std::cout<<"Starting plane intersection\n";

	//In this case ray is in plane or parallel to it
	float rayNormal = dot(ray.d, normal);

	if (rayNormal == 0.0f) {
		return Intersection::failure();
	}

	float dist = (dot(op,normal))/ dot(ray.d, normal);

	if (dist < previousBestDistance && dist>0) {

        //std::cout<<"Inside the if";

		Point ip = ray.getPoint(dist);

		return Intersection(dist,ray,this,this->normal,ip);
	}
	
	return Intersection::failure();


}

Solid::Sample InfinitePlane::sample() const {
    /* TODO */ NOT_IMPLEMENTED;
}

float InfinitePlane::getArea() const {
    /* TODO */ NOT_IMPLEMENTED;
}

}
