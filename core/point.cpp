#include <core/point.h>
#include <core/float4.h>
#include <core/scalar.h>
#include <core/assert.h>
#include <core/vector.h>


namespace rt {

Point::Point(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Point::Point(const Float4& f4)
{
	// if(fabs(f4.w - 1.0f) > __FLT_EPSILON__){
	// 	throw("Invalid arguments given");
	// }

	float invW = 1 / f4.w;

	this->x = f4.x * invW;
	this->y = f4.y * invW;
	this->z = f4.z * invW;
}

Vector Point::operator - (const Point& b) const {

	return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

Point operator+(const Point &a, const Point &b) {
        return {a.x + b.x, a.y + b.y, a.z + b.z};
}


bool Point::operator == (const Point& b) const {
	return (fabs(this->x - b.x) <= __FLT_EPSILON__) && (fabs(this->y - b.y) <= __FLT_EPSILON__) && (fabs(this->z - b.z) <= __FLT_EPSILON__);
}

bool Point::operator != (const Point& b) const {
	return (fabs(this->x - b.x) > __FLT_EPSILON__) || (fabs(this->y - b.y) > __FLT_EPSILON__) || (fabs(this->z - b.z) > __FLT_EPSILON__);
}

float Point::operator [] (const int dim) const{
	switch(dim){
		case(0):
			return this->x;
		case(1):
			return this->y;
		case(2):
			return this->z;
	}
}

Point operator * (float scalar, const Point& b) {
	return Point(b.x * scalar, b.y * scalar, b.z * scalar);
}

Point operator * (const Point& a, float scalar) {
    return Point(a.x * scalar, a.y * scalar, a.z * scalar);
}

Point min(const Point& a, const Point& b) {
	return Point(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

Point max(const Point& a, const Point& b) {
	return Point(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

}
