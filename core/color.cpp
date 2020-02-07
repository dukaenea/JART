#include <core/color.h>
#include <core/scalar.h>
#include <core/float4.h>

namespace rt {

RGBColor::RGBColor(const Float4& f4)
{
	/* TODO */ NOT_IMPLEMENTED;
}

RGBColor RGBColor::operator + (const RGBColor& c) const {
	return RGBColor(this->r + c.r, this->g + c.g, this->b + c.b);
}

RGBColor RGBColor::operator - (const RGBColor& c) const {
	return RGBColor(this->r - c.r, this->g - c.g, this->b - c.b);
}

RGBColor RGBColor::operator * (const RGBColor& c) const {
	return RGBColor(this->r * c.r, this->g * c.g, this->b * c.b);
}

bool RGBColor::operator == (const RGBColor& c) const {
	return this->r == c.r && this->g == c.g && this->b == c.b;
}

bool RGBColor::operator != (const RGBColor& b) const {
	return this->r != b.r || this->g != b.g || this->b != b.b;
}

RGBColor RGBColor::clamp() const {

	float lo = 0.0f;
	float hi = 1.0f;

	float red = (this->r < lo) ? lo : (this->r > hi) ? hi : this->r;
	float green = (this->g < lo) ? lo : (this->g > hi) ? hi : this->g;
	float blue = (this->b < lo) ? lo : (this->b > hi) ? hi : this->b;

	return RGBColor(red, green, blue);
}

RGBColor RGBColor::gamma(float gam) const {
    /* TODO */ NOT_IMPLEMENTED;
}

float RGBColor::luminance() const {
    /* TODO */ NOT_IMPLEMENTED;
}

RGBColor operator * (float scalar, const RGBColor& c) {
	return RGBColor(scalar * c.r, scalar * c.g, scalar * c.b);
}

RGBColor operator * (const RGBColor& c, float scalar) {
	return RGBColor(scalar * c.r, scalar * c.g, scalar * c.b);
}

RGBColor operator / (const RGBColor& c, float scalar) {
	if (scalar == 0.0f) {
		std::cout<<"Division by 0.0 is not allowed."<<std::endl;
		throw "Division by 0.0 is not allowed.";
	}

	return RGBColor(c.r / scalar, c.g / scalar, c.b / scalar);
}

}
