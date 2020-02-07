#include <rt/materials/phong.h>
#include <rt/textures/texture.h>
#include <core/scalar.h>
#include <math.h>

namespace rt {

PhongMaterial::PhongMaterial(Texture* specular, float exponent)
{
    this->specular = specular;
    this->exponent = exponent;
}

RGBColor PhongMaterial::getReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir, const Vector& inDir) const {
    //cos between the incomming ray direction and surface normal
    float cosIN = dot(inDir.normalize(), normal.normalize());
    //calculating the H vector that is half way between inDir and outDir
    Vector r = inDir - 2*(cosIN)*normal;
    //calculating the cos between the reflected ray and the out direction
    float cosRO = dot(r.normalize(), outDir.normalize());
    RGBColor textureColor = this->specular->getColor(texPoint) * (this->exponent+2) / (2 * pi); //normalizing

    return textureColor * std::pow(std::max(cosRO, 0.0f), this->exponent) * cosIN;

}

RGBColor PhongMaterial::getEmission(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
    return RGBColor::rep(0);
}

Material::SampleReflectance PhongMaterial::getSampleReflectance(const Point& texPoint, const Vector& normal, const Vector& outDir) const {
	UNREACHABLE;
}

Material::Sampling PhongMaterial::useSampling() const {
	return SAMPLING_NOT_NEEDED;
}

}