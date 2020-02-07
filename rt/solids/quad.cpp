#include <rt/solids/quad.h>
#include <rt/solids/triangle.h>

namespace rt {

Quad::Quad(const Point& origin, const Vector& span1, const Vector& span2, CoordMapper* texMapper, Material* material)
{
    this->origin= origin;
	this->span1 = span1;
	this->span2 = span2;
	this->texMapper = texMapper;
	this->material = material;
}

BBox Quad::getBounds() const {
    // finding the other three points of the quad
	Point p1 = this->origin + this->span1;
	Point p2 = this->origin + this->span2;
	Point p3 = this->origin + (this->span1 + this->span2);


	return BBox(
		Point(
			std::min(std::min(p1.x, p2.x), std::min(this->origin.x, p3.x)),
			std::min(std::min(p1.y, p2.y), std::min(this->origin.y, p3.y)),
			std::min(std::min(p1.z, p2.z), std::min(this->origin.z, p3.z))
		),
		Point(
			std::max(std::max(p1.x, p2.x), std::max(this->origin.x, p3.x)),
			std::max(std::max(p1.y, p2.y), std::max(this->origin.y, p3.y)),
			std::max(std::max(p1.z, p2.z), std::max(this->origin.z, p3.z))
		)
	);




}

Point Quad::getLocalIntersectionPoint(const Ray& ray) const{
	// The first component of the point represents the projection of the intersection point on span1
	// The second component of the point represents the projection of the intersection point on span2

	//Computing the global intersection point
	Vector normal = cross(span1, span2).normalize();
	float t = dot(this->origin - ray.o, normal) / dot(ray.d, normal);
	Point globalIntersectionPoint = ray.getPoint(t);


	return Point(dot((globalIntersectionPoint - this->origin), span1.normalize())/span1.length(),
				dot((globalIntersectionPoint - this->origin), span2.normalize())/span2.length(),
				0
		);
}

Intersection Quad::intersect(const Ray& ray, float previousBestDistance) const {
    
    Triangle t1 = Triangle(origin, this->span1 + origin, this->span2 + origin,this->texMapper,this->material);
	
	Triangle t2 = Triangle(this->span2 + origin,this->span1 + this->span2 + origin,this->span1 + origin, this->texMapper, this->material);


	Intersection t1Intersection = t1.intersect(ray, previousBestDistance);

	if (!t1Intersection){ 
		//std::cout<<"In the second triangle\n\n";
		Intersection t2Intersection = t2.intersect(ray, previousBestDistance);
		if(t2Intersection){
			if(t2Intersection.distance > previousBestDistance ||  t2Intersection.distance < 0){
				return Intersection::failure();
			}
			t2Intersection.nrm = -t2Intersection.normal();
			t2Intersection.loc = this->getLocalIntersectionPoint(ray);
			t2Intersection.solid = this;
			return t2Intersection;
		}
		else{
			return Intersection::failure();
		}
	}
	else{
		if(t1Intersection.distance > previousBestDistance ||  t1Intersection.distance < 0){
				return Intersection::failure();
			}

		t1Intersection.loc = this->getLocalIntersectionPoint(ray);
		t1Intersection.solid = this;
		
		return t1Intersection;
	}
}

Solid::Sample Quad::sample() const {
    Solid::Sample samplePoint;
	//since there is no curvature on the quad, we can get the normal at any point in it as follows
	Vector normal = cross(this->span1, this->span2).normalize();
	samplePoint.normal = normal;

	//to get a random point inside the quad we generate two random values [0,1]
	//and use the to scale each spaning vector. Our random point will be at the end of the sum resulting vector
	float rnd1 = random();
	float rnd2 = random();


	Vector v = Vector(this->origin.x,this->origin.y,this->origin.z) + (this->span1 * rnd1 + this->span2 * rnd2); 
	samplePoint.point = Point(v.x, v.y, v.z);

	return samplePoint;
}

float Quad::getArea() const {
   return this->span1.length() *  this->span2.length();
}

}
