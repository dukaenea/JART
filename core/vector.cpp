#include <core/scalar.h>
#include <core/vector.h>
#include <core/point.h>
#include <core/float4.h>
#include <core/assert.h>
#include <algorithm>
#include <cmath>

namespace rt {

Vector::Vector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector::Vector(const Float4& f4)
{
    // if(fabs(f4.w - 0.0f) > __FLT_EPSILON__){
	// 	throw("Invalid arguments given");
	// }

	this->x = f4.x;
	this->y = f4.y;
	this->z = f4.z;
}

Vector Vector::operator + (const Vector& b) const {
	return Vector(this->x + b.x, this->y + b.y, this->z + b.z);
}

Vector Vector::operator - (const Vector& b) const {
	return Vector(this->x - b.x, this->y - b.y, this->z - b.z);
}

Vector Vector::operator - () const {
	return Vector(-this->x, -this->y, -this->z);
}

Vector Vector::normalize() const {
	float length = this->length();

	if (length == 0) {
		return Vector(0.0f, 0.0f, 0.0f);
	}

	float lengthInverse = 1 / length; //so we do not do three divisions.
	return Vector(this->x * lengthInverse, this->y * lengthInverse, this->z * lengthInverse);
}

Vector operator * (float scalar, const Vector& b) {
	return Vector(b.x * scalar, b.y * scalar, b.z * scalar);
}

Vector operator * (const Vector& a, float scalar) {
	return Vector(a.x * scalar, a.y * scalar, a.z * scalar);
}

Vector operator / (const Vector& a, float scalar) {
	if (scalar == 0.0f) {
		throw "Division by 0.0 is not allowed";
	}

	float scalarInverse = 1 / scalar; //so we do not have to devide three times.
	return Vector(a.x * scalarInverse, a.y * scalarInverse, a.z * scalarInverse);
}

Vector cross(const Vector& a, const Vector& b) {
    //compute each component separately
	float cx = a.y * b.z - a.z * b.y;
	float cy = a.z * b.x - a.x * b.z;
	float cz = a.x * b.y - a.y * b.x;

	return Vector(cx, cy, cz);
}

float dot(const Vector& a, const Vector& b) {
	return a.x* b.x + a.y * b.y + a.z * b.z;
}

float Vector::lensqr() const {
	return (this->x * this->x) + (this->y * this->y) + (this->z * this->z);
}

float Vector::length() const {
	return sqrtf((this->x * this->x) + (this->y * this->y) + (this->z * this->z));
}


bool Vector::operator == (const Vector& b) const {
	return (fabs(this->x - b.x) <= __FLT_EPSILON__) && (fabs(this->y - b.y) <= __FLT_EPSILON__) && (fabs(this->z - b.z) <= __FLT_EPSILON__);
}

bool Vector::operator != (const Vector& b) const {
	return (fabs(this->x - b.x) > __FLT_EPSILON__) || (fabs(this->y - b.y) > __FLT_EPSILON__) || (fabs(this->z - b.z) > __FLT_EPSILON__);
}

Vector min(const Vector& a, const Vector& b) {
	return Vector(min(a.x, b.x), min(a.y, b.y), min(a.z, b.z));
}

Vector max(const Vector& a, const Vector& b) {
	return Vector(max(a.x, b.x), max(a.y, b.y), max(a.z, b.z));
}

Point operator + (const Point& a, const Vector& b) {
    return Point(a.x+b.x, a.y+b.y, a.z+b.z);
}

Point operator + (const Vector& a, const Point& b) {
	return Point(a.x + b.x, a.y + b.y, a.z + b.z);
}

Point operator - (const Point& a, const Vector& b) {
	return Point(a.x - b.x, a.y - b.y, a.z - b.z);
}

Point operator * (const Float4& scale, const Point& p) {
	return Point(
		p.x * scale.x / scale.w,
		p.y * scale.y / scale.w,
		p.z * scale.z / scale.w
	);
}

}
